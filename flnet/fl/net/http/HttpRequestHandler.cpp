#include "fl/net/http/HttpRequestHandler.hpp"
#include "fl/utils/Log.hpp"

#include <algorithm>
#include <execution>

std::mutex handlerMutex;

namespace fl {

    HttpRequestHandler::HttpRequestHandler(Ref<HttpRouter> const& router)
        : router_(router) 
    {
        bad_request_ = 
        [](HttpRequest const& req, http::status status)
        {
            HttpResponse res{ status, req.Version() };
            res.SetHeader(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.SetHeader(http::field::content_type, "text/html");
            res.SetAlive(req.Alive());
            res.Body() = "Can not find page or resource";
            res.Prepare();
            return res;
        };
    }

    void HttpRequestHandler::SetBadHandler(BadRequest const& method)
    {
        bad_request_ = method;
    }

    void HttpRequestHandler::AddRouteHandler(HttpRequestHandler::Data const& data)
    {
        handlers_.push_back(data);
        auto& t = handlers_.back();
        t.Target = router_->GetPreparedTarget(data.Target);
    }

    http::message_generator HttpRequestHandler::HandleRequest(HttpRequest&& req) const 
    {
        std::lock_guard<std::mutex> lock(handlerMutex);

        auto url = req.Url();
        auto request_method = req.Method();

        if (!url.IsValid())
            return bad_request_(req, http::status::bad_request);

        if (!HttpRouter::IsTargetLegal(url.Target()))
            return bad_request_(req, http::status::bad_request);

        if (router_->IsContent(url.Target()))        // if user asked for content
        {
            std::string path = router_->GetContentPath(url.Target());

            beast::error_code ec;
            http::file_body::value_type body;
            body.open(path.c_str(), beast::file_mode::scan, ec);

            if(ec == beast::errc::no_such_file_or_directory)
                return bad_request_(req, http::status::not_found);

            if(ec)
                return bad_request_(req, http::status::internal_server_error);

            HttpResponseFile resFile{http::status::ok, req.Version()};
            resFile.SetHeader(http::field::server, BOOST_BEAST_VERSION_STRING);
            resFile.SetAlive(req.Alive());
            resFile.SetHeader(http::field::content_type, MimeType::FromString(path).GetFormat());
            resFile.SetHeader(http::field::content_type, MimeType::FromString(path).GetFormat());
            resFile.Body() = std::move(body);
            resFile.Prepare();

            return std::move(resFile);                 
        }

        auto method_handler_only = 
        [&](Data const& data) {
            bool is_method;         
            if (data.Method.has_value())
                is_method = data.Method == request_method;

            return data.Target.empty() 
                && data.Handler.has_value()
                && is_method;
        };

        auto target_only = 
        [&](Data const& data) {
            bool is_target = data.Target == router_->GetPreparedTarget(url.Target());
            return is_target 
                && !data.Handler.has_value()
                && !data.Method.has_value();
        };

        auto target_handler_only = 
        [&](Data const& data) {
            bool is_target = data.Target == router_->GetPreparedTarget(url.Target());
            return is_target 
                && data.Handler.has_value()
                && !data.Method.has_value();
        };

        auto all_handler = 
        [&](Data const& data) {
            bool is_target = data.Target == router_->GetPreparedTarget(url.Target());
            bool is_method;         
            if (data.Method.has_value())
                is_method = data.Method == request_method;

            return is_target 
                && is_method
                && data.Handler.has_value();
        };

        if (!router_->IsTarget(url.Target()))              
            return bad_request_(req, http::status::not_found);  

        std::string path = router_->GetRoutePath(url.Target());
        std::ifstream file(path);

        if(!file.is_open())
            return bad_request_(req, http::status::internal_server_error);

        std::string file_content((std::istreambuf_iterator<char>(file)),
                                std::istreambuf_iterator<char>());

        file.close();

        std::vector<Data> routed_callbacks;

        std::copy_if(handlers_.begin(), handlers_.end(), std::back_inserter(routed_callbacks), method_handler_only);
        std::copy_if(handlers_.begin(), handlers_.end(), std::back_inserter(routed_callbacks), target_handler_only);
        std::copy_if(handlers_.begin(), handlers_.end(), std::back_inserter(routed_callbacks), target_only);
        std::copy_if(handlers_.begin(), handlers_.end(), std::back_inserter(routed_callbacks), all_handler);

        HttpResponse resHtml {http::status::ok, req.Version()};
        resHtml.SetHeader(http::field::server, BOOST_BEAST_VERSION_STRING);
        resHtml.SetAlive(req.Alive());
        resHtml.SetHeader(http::field::content_type, MimeType::FromString(path).GetFormat());
        resHtml.SetSize(file_content.size());
        resHtml.Body() = std::move(file_content);

        std::for_each(std::execution::par, routed_callbacks.begin(), routed_callbacks.end(), 
        [&req, &resHtml](Data const& data) {
            if (data.Handler.has_value())
                resHtml = data.Handler.value()(req, std::move(resHtml));
        });

        return std::move(resHtml);
    }
}