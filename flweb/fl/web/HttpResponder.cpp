#include "fl/web/HttpResponder.hpp"
#include "fl/core/Log.hpp"

#include <algorithm>
#include <execution>

namespace Forward::Web {

    HttpResponseString DefaultBadRequestCallback(
        HttpRequestString const& req, 
        HttpStatus status)
    {
        HttpResponseString res(status, req.Base().version());
        res.Base().set(HttpField::server, BOOST_BEAST_VERSION_STRING);
        res.Base().set(HttpField::content_type, MimeType::FromString("html").GetMimeName());
        res.Base().keep_alive(req.Base().keep_alive());
        res.Base().body() = "Can not find page or resource";
        res.Base().prepare_payload();

        return std::move(res);
    }

    void LoadWebPageContent(
        std::string_view path, 
        HttpResponseFile& res, 
        Core::ErrorCode& ex)
    {   
        Core::FileBody::value_type body;
        body.open(path.data(), Core::Beast::file_mode::scan, ex);

        if (ex)
            return;

        uint64_t size = body.size();

        res.Base().set(HttpField::content_type, MimeType::FromString(path).GetMimeName());
        res.Base().body() = std::move(body);
        res.Base().content_length(size);
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

    void HttpResponder::SetBadHandler(BadRequest const& method)
    {
        std::unique_lock lock(mtx_);
        bad_request_ = method;
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

    Core::MessageGenerator HttpResponder::HandleRequest(HttpRequestString&& req)
    {
        auto url = req.Url();

        if (!url.IsValid())
        {
            return bad_request_(req, HttpStatus::bad_request);
        }

        auto target = url.Path();

        if (!HttpUrl::IsPathLegal(target))
        {
            return bad_request_(req, HttpStatus::bad_request);
        }

        if (!router_)
        {
            return HandleWithoutRouter(std::move(req));
        }
        else
        {
            return HandleWithRouter(std::move(req));
        }
    }

    Core::MessageGenerator HttpResponder::HandleWithoutRouter(HttpRequestString&& req)
    {
        auto url = req.Url();
        auto target = url.Path();
        auto method = req.Base().method();

        HttpResponse res(HttpStatus::ok, req.Base().version());
        res.Base().keep_alive(req.Base().keep_alive());
        res.Base().set(HttpField::server, BOOST_BEAST_VERSION_STRING);

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
        {
            return bad_request_(req, HttpStatus::not_found);
        }

        auto const& handler = *handler_it->second.Handler;

        return handler(req, std::move(res));
    }

    Core::MessageGenerator HttpResponder::HandleWithRouter(HttpRequestString&& req)
    {
        auto url = req.Url();
        auto target = url.Path();
        auto method = req.Base().method();

        auto prep_target = router_->GetPreparedTarget(target);

        bool is_target = router_->IsTarget(prep_target);
        bool is_content = router_->IsContent(target);

        if (!is_target && !is_content)
        {
            return bad_request_(req, HttpStatus::not_found);
        }

        auto const& only_target_it = std::find_if(handlers_.cbegin(), handlers_.cend(),
        [&is_target, &method](std::pair<std::string, RouteData> const& pair)
        {
            auto const& [target, route_data] = pair;

            return (is_target 
                && (!route_data.Method.has_value() 
                    || *route_data.Method == method)
                && !route_data.Handler.has_value());
        });

        bool is_only_target = only_target_it != handlers_.cend();

        if (is_only_target || is_content)
        {
            HttpResponseFile resFile(HttpStatus::ok, req.Base().version());
            resFile.Base().keep_alive(req.Base().keep_alive());
            resFile.Base().set(HttpField::server, BOOST_BEAST_VERSION_STRING);

            std::string path = is_target ? router_->GetRouteFilePath(target) : router_->GetContentFilePath(target);

            Core::ErrorCode ex;
            LoadWebPageContent(path, resFile, ex);

            if (ex == Core::Beast::errc::no_such_file_or_directory)
            {
                return bad_request_(req, HttpStatus::not_found);
            }

            if (ex)
            {
                return bad_request_(req, HttpStatus::internal_server_error);
            }

            return resFile;
        }

        // section where only callback

        auto const& only_handler_it = std::find_if(handlers_.cbegin(), handlers_.cend(),
        [&prep_target, &method](std::pair<std::string, RouteData> const& pair)
        {
            auto const& [target, route_data] = pair;

            return (prep_target == target
                && route_data.Method.has_value()
                && *route_data.Method == method
                && route_data.Handler.has_value());
        });

        bool is_only_handler = only_handler_it != handlers_.cend();

        if (!is_only_handler)
        {
            return bad_request_(req, HttpStatus::internal_server_error);
        }

        auto const& handler = *only_handler_it->second.Handler;

        HttpResponse res(HttpStatus::ok, req.Base().version());
        res.Base().keep_alive(req.Base().keep_alive());
        res.Base().set(HttpField::server, BOOST_BEAST_VERSION_STRING);
        res = handler(req, std::move(res));
        res.Base().prepare_payload();

        return res;
    }

}