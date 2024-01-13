#pragma once

#include "fl/web/HttpRequest.hpp"
#include "fl/web/HttpResponse.hpp"

#include "fl/web/HttpRouter.hpp"

namespace Forward::Web {

    class HttpResponder
    {
    public:
        using Callback = std::function<HttpResponseString(HttpRequestString const&, HttpResponseString&&)>;
        using BadRequest = std::function<HttpResponseString(HttpRequestString const&, HttpStatus)>;

        struct RouteData
        {
            std::string Target;
            std::optional<HttpMethod> Method;
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
         *  @param method 
         * 
         *  sets callback when issue appear
         */
        void SetBadHandler(BadRequest const& method);
        /**
         *  @param data handler data
         *  
         *  
         */
        void AddRouteHandler(HttpResponder::RouteData const& route);

        /**
         *   @param req incoming request
         *  
         *   accepts incoming request
         */
        Core::MessageGenerator HandleRequest(HttpRequestString&& req);

    private:
        Core::MessageGenerator HandleWithRouter(HttpRequestString&& req);
        Core::MessageGenerator HandleWithoutRouter(HttpRequestString&& req);

    };
} // namespace Forward::Web