#pragma once

#include "fl/net/SSLServer.hpp"

#include "fl/web/HttpResponder.hpp"
#include "fl/web/HttpSession.hpp"

namespace Forward::Web {

    class HttpServer final : public Net::SslServer
    {
    private:
        Ref<HttpRouter> router_;

    protected:
        Ref<HttpResponder> responder_;

    public:
        HttpServer(uint32_t io_count);
        HttpServer(Core::SSL::Method method, uint32_t io_count);
        HttpServer(std::string_view web_dir, Core::SSL::Method method, uint32_t io_count = 1);

        virtual ~HttpServer() override;

        void SetBadRequest(HttpResponder::BadRequest const& handler);
        void SetContentFolders(std::vector<std::string> const& folders);

        template<typename ...Args>
        constexpr void Route(Args&& ...args) 
        {
            HttpResponder::RouteData route {std::forward<Args>(args)...};

            if (!route.Target.empty())
                if (router_)
                    router_->RegisterTarget(route.Target);

            if (!route.Target.empty()
                || route.Method.has_value()
                && route.Callback.has_value())
                responder_->AddRouteHandler(route);
        }
    };
}