#pragma once

#include "fl/net/http/HttpRequest.hpp"
#include "fl/net/http/HttpResponse.hpp"

#include "fl/net/http/HttpRouter.hpp"

namespace Forward::Web {

    class HttpResponder
    {
    public:
        using Callback = std::function<HttpResponse(HttpRequest const&, HttpResponse&&)>;
        using BadRequest = std::function<HttpResponse(HttpRequest const&, Http::status)>;

        struct RouteData
        {
            std::string Target;
            std::optional<Http::verb> Method;
            std::optional<HttpResponder::Callback> Handler;
        };

    private:
        Ref<HttpRouter const> router_ = nullptr;

        BadRequest bad_request_;
        std::unordered_multimap<std::string, RouteData> handlers_;

        mutable std::shared_mutex mtx_;

    public:
        HttpResponder();
        /**
         *  @param router valid router
         * 
         *  can be inited with router to return pages/content paths  
         */
        HttpResponder(Ref<HttpRouter const> const& router);

        /**
         *  @param data handler data
         *  
         *  
         */
        void AddRouteHandler(HttpResponder::RouteData const& route);
        /**
         *  @param method 
         * 
         *  sets callback when issue appear
         */
        void SetBadHandler(BadRequest const& method);
        /**
         *   @param req incoming request
         *  
         *   accepts incoming request
         */
        Http::message_generator HandleRequest(HttpRequest&& req) const;
    };
}