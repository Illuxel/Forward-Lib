#pragma once

#include "fl/net/http/HttpRequest.hpp"
#include "fl/net/http/HttpResponse.hpp"

#include "fl/net/http/HttpRouter.hpp"

namespace fl {

    class HttpRequestHandler
    {
    public:
        using Callback = std::function<HttpResponse(HttpRequest const&, HttpResponse&&)>;
        using BadRequest = std::function<HttpResponse(HttpRequest const&, http::status)>;

        struct Data 
        {
            std::string Target;
            std::optional<http::verb> Method;
            std::optional<Callback> Handler;
        };

    private:
        Ref<HttpRouter> router_;
        BadRequest bad_request_;

        std::vector<Data> handlers_;

    public:
        HttpRequestHandler(Ref<HttpRouter> const& router);

        void AddRouteHandler(HttpRequestHandler::Data const& data);
        void SetBadHandler(BadRequest const& method);

        http::message_generator HandleRequest(HttpRequest&& req) const;
    };
}