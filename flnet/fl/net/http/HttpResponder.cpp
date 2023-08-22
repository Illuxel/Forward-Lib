#include "fl/net/http/HttpResponder.hpp"
#include "fl/utils/Log.hpp"

#include <algorithm>
#include <execution>

namespace Forward::Web {

    HttpResponse DefaultBadRequestCallback(
        Http::request<Http::string_body> const& req, 
        Http::status status)
    {
        Http::response<Http::string_body> res(status, req.version());
        res.set(Http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(Http::field::content_type, MimeType::FromString("html").GetMimeName());
        res.keep_alive(req.keep_alive());
        res.body() = "Can not find page or resource";
        res.prepare_payload();

        return std::move(res);
    }

    void LoadWebPageContent(
        std::string_view path, 
        Http::response<Http::file_body>& res, 
        Core::Beast::error_code& ec)
    {   
        Http::file_body::value_type body;
        body.open(path.data(), Core::Beast::file_mode::scan, ec);

        if (ec)
            return;

        uint64_t size = body.size();

        res.set(Http::field::content_type, MimeType::FromString(path).GetMimeName());
        res.body() = std::move(body);
        res.content_length(size);
    }

    HttpResponder::HttpResponder()
    {
        bad_request_ = DefaultBadRequestCallback;
    }
    HttpResponder::HttpResponder(Ref<HttpRouter const> const& router)
        : router_(router) 
    {
        bad_request_ = DefaultBadRequestCallback;
    }

    void HttpResponder::AddRouteHandler(HttpResponder::RouteData const& data)
    {
        if (!router_) 
        {
            handlers_.emplace(data.Target, data);
        }
        else 
        {
            handlers_.emplace(
                router_->GetPreparedTarget(data.Target),
                data
            );
        }
    }

    void HttpResponder::SetBadHandler(BadRequest const& method)
    {
        std::unique_lock lock(mtx_);
        bad_request_ = method;
    }

    Http::message_generator HttpResponder::HandleRequest(HttpRequest&& req) const
    {
        HttpUrl url = req.Url();

        if (!url.IsValid())
            return bad_request_(req, Http::status::bad_request);

        std::string target = url.Path();
        Http::verb method = req.base().method();
        
        if (!HttpUrl::IsPathLegal(target))
            return bad_request_(req, Http::status::bad_request);

        // without router 
        if (!router_)
        {
            HttpResponse res(Http::status::ok, req.base().version());
            res.base().keep_alive(req.base().keep_alive());
            res.base().set(Http::field::server, BOOST_BEAST_VERSION_STRING);

            std::shared_lock lock(mtx_);

            auto const& handler_it = std::find_if(handlers_.cbegin(), handlers_.cend(), 
            [target, method](std::pair<std::string, RouteData> const& pair)
            {
                RouteData const& route = pair.second;
                std::string handler_target = pair.first;

                if (target == route.Target
                    && method == route.Method
                    && route.Handler.has_value())
                    return true;

                return false;
            });

            if (handler_it == handlers_.cend())
                return bad_request_(req, Http::status::not_found);

            auto const& handler = *handler_it->second.Handler;

            return handler(req, std::move(res));
        }

        // with router

        std::string prep_target = router_->GetPreparedTarget(target);

        bool is_target = router_->IsTarget(prep_target);
        bool is_content = router_->IsContent(target);

        if (!is_target && !is_content)
            return bad_request_(req, Http::status::not_found);

        auto const& only_target_it = std::find_if(handlers_.cbegin(), handlers_.cend(),
        [&is_target, &method](std::pair<std::string, RouteData> const& pair)
        {
            auto const& h_method = pair.second.Method;
            auto const& h_callback = pair.second.Handler;

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

        bool is_only_target = only_target_it != handlers_.cend();

        if (is_only_target || is_content)
        {
            Http::response<Http::file_body> resFile(Http::status::ok, req.base().version());

            resFile.keep_alive(req.base().keep_alive());
            resFile.set(Http::field::server, BOOST_BEAST_VERSION_STRING);
                        
            std::string path;

            if (is_target)
                path = router_->GetRouteFilePath(target);

            if (is_content)
                path = router_->GetContentFilePath(target);

            Core::Error ec;

            LoadWebPageContent(path, resFile, ec);

            if(ec == Core::Beast::errc::no_such_file_or_directory) 
                return bad_request_(req, Http::status::not_found);

            if (ec)
                return bad_request_(req, Http::status::internal_server_error);

            return resFile;
        }

        // section where only callback

        auto const& only_handler_it = std::find_if(handlers_.cbegin(), handlers_.cend(), 
        [&prep_target, &method](std::pair<std::string, RouteData> const& pair)
        {
            auto const& h_method = pair.second.Method;
            auto const& h_callback = pair.second.Handler;

            if (prep_target == pair.first
                && h_method != std::nullopt
                && *h_method == method
                && h_callback != std::nullopt)
                return true;

            return false;
        });

        bool is_only_handler = only_handler_it != handlers_.cend();

        if (!is_only_handler)
            return bad_request_(req, Http::status::internal_server_error);

        auto const& handler = *only_handler_it->second.Handler;

        HttpResponse res(Http::status::ok, req.base().version());
        res.base().keep_alive(req.base().keep_alive());
        res.base().set(Http::field::server, BOOST_BEAST_VERSION_STRING);
        res = handler(req, std::move(res));
        res.base().prepare_payload();

        return res;
    }
}