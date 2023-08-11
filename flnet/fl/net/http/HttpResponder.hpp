#pragma once

#include "fl/net/http/HttpRequest.hpp"
#include "fl/net/http/HttpResponse.hpp"

#include "fl/net/http/HttpRouter.hpp"

namespace Forward {

    class HttpResponder
    {
    public:
        using CallBack = std::function<HttpResponse(HttpRequest const&, HttpResponse&&)>;
        using BadRequest = std::function<HttpResponse(HttpRequest const&, http::status)>;

        struct HandlerData
        {
            std::string Target;
            std::optional<http::verb> Method;
            std::optional<CallBack> Callback;
        };

    private:
        Ref<HttpRouter const> router_ = nullptr;

        BadRequest bad_request_;
        std::unordered_multimap<std::string, HandlerData> handlers_;

        mutable std::shared_mutex res_mtx_;

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
        void AddRouteHandler(HttpResponder::HandlerData const& data);
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
        http::message_generator HandleRequest(HttpRequest&& req) const;
    };
}