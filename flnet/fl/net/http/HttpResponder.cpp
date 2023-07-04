#include "fl/net/http/HttpResponder.hpp"
#include "fl/utils/Log.hpp"

#include <algorithm>
#include <execution>

namespace fl {

    HttpResponse DefaultBadRequestCallBack(HttpRequest const& req, http::status status)
    {
        HttpResponse res(status, req.Base().version());

        res.Base().set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.Base().set(http::field::content_type, MimeType::FromString("html").GetMimeName());
        res.Base().keep_alive(req.Base().keep_alive());
        res.Base().body() = "Can not find page or resource";
        res.Base().prepare_payload();

        return std::move(res);
    }

    void LoadWebPageContent(std::string const& path, HttpResponseFile& res, beast::error_code ec)
    {   
        http::file_body::value_type body;
        body.open(path.c_str(), beast::file_mode::scan, ec);

        if(ec)
            return;

        auto const size = body.size();

        res.Base().set(http::field::content_type, MimeType::FromString(path).GetMimeName());
        res.Base().body() = std::move(body);
        res.Base().content_length(size);  

        body.close();
    }

    HttpResponder::HttpResponder()
    {
        bad_request_ = DefaultBadRequestCallBack;
    }
    HttpResponder::HttpResponder(Ref<HttpRouter const> const& router)
        : router_(router) 
    {
        bad_request_ = DefaultBadRequestCallBack;
    }

    void HttpResponder::AddRouteHandler(HttpResponder::HandlerData const& data)
    {
        if (!router_) 
        {
            handlers_.emplace(data.Target, std::move(data));
            return;
        }

        handlers_.emplace(
            router_->GetPreparedTarget(data.Target),
            std::move(data)
        );
    }

    void HttpResponder::SetBadHandler(BadRequest const& method)
    {
        bad_request_ = method;
    }

    http::message_generator HttpResponder::HandleRequest(HttpRequest&& req) const 
    {
        auto url = req.Url();
        auto method = req.Base().method();
        auto target = url.Target();
        
        if (!url.IsValid())
            return bad_request_(req, http::status::bad_request);

        if (!HttpUrl::IsTargetLegal(target))
            return bad_request_(req, http::status::bad_request);

        std::lock_guard<std::mutex> lock(mutex_);

        // without router 
        if (!router_)
        {
            HttpResponse res(http::status::ok, req.Base().version());
            res.Base().keep_alive(req.Base().keep_alive());
            res.Base().set(http::field::server, BOOST_BEAST_VERSION_STRING);

            auto const& handler_it = std::find_if(handlers_.begin(), handlers_.end(), 
            [&target, &method](std::pair<std::string, HandlerData> const& pair)
            {
                HandlerData const& handler = pair.second;
                std::string handler_target = pair.first;

                if (target == handler.Target
                    && method == handler.Method
                    && handler.Callback.has_value())
                    return true;

                return false;
            });

            if (handler_it == handlers_.end())
                return bad_request_(req, http::status::not_found);

            auto const& handler = *handler_it->second.Callback;
            return handler(req, std::move(res));
        }

        // with router

        std::string prep_target = router_->GetPreparedTarget(target);

        bool is_target = router_->IsTarget(prep_target);
        bool is_content = router_->IsContent(target);

        if (!is_target && !is_content)
            return bad_request_(req, http::status::not_found);

        auto const& only_target_it = std::find_if(handlers_.begin(), handlers_.end(),
        [&is_target, &method](std::pair<std::string, HandlerData> const& pair)
        {
            auto const& h_method = pair.second.Method;
            auto const& h_callback = pair.second.Callback;

            if (is_target 
                && h_method == std::nullopt
                && h_callback == std::nullopt)
                return true;

            if (is_target 
                && h_method.has_value()
                && *h_method == method
                && h_callback == std::nullopt)
                return true;

            return false;
        });

        bool is_only_target = only_target_it != handlers_.end();

        if (is_only_target || is_content)
        {
            beast::error_code ec;
            HttpResponseFile resFile(http::status::ok, req.Base().version());

            resFile.Base().keep_alive(req.Base().keep_alive());
            resFile.Base().set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        
            std::string path;

            if (is_content)
                path = router_->GetContentPath(target);

            if (is_target)
                path = router_->GetRoutePath(target);

            LoadWebPageContent(path, resFile, ec);

            if(ec == beast::errc::no_such_file_or_directory) 
                return bad_request_(req, http::status::not_found);

            if (ec)
                return bad_request_(req, http::status::internal_server_error);

            return resFile;
        }

        // section where only callback

        auto const& only_handler_it = std::find_if(handlers_.begin(), handlers_.end(), 
        [&prep_target, &method](std::pair<std::string, HandlerData> const& pair)
        {
            auto const& h_method = pair.second.Method;
            auto const& h_callback = pair.second.Callback;

            if (prep_target == pair.first
                && h_method != std::nullopt
                && *h_method == method
                && h_callback != std::nullopt)
                return true;

            return false;
        });

        bool is_only_handler = only_handler_it != handlers_.end();

        if (!is_only_handler)
            return bad_request_(req, http::status::internal_server_error);

        auto const& handler = *only_handler_it->second.Callback;

        HttpResponse res(http::status::ok, req.Base().version());
        res.Base().keep_alive(req.Base().keep_alive());
        res.Base().set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res = handler(req, std::move(res));
        res.Base().prepare_payload();

        return res;
    }
}