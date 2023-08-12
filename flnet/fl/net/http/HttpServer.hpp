#pragma once

#include "fl/net/SslServer.hpp"

#include "fl/net/http/HttpResponder.hpp"
#include "fl/net/http/HttpSession.hpp"

namespace Forward::Web {

    class HttpServer final : public Net::SslServer
    {
    private:
        Ref<HttpRouter> router_;

    protected:
        Ref<HttpResponder> responder_;

    public:
        HttpServer(Core::Ssl::context::method method, uint32_t io_count = 1);
        HttpServer(std::string_view web_dir, Core::Ssl::context::method method, uint32_t io_count = 1);

        void SetBadRequest(HttpResponder::BadRequest const& handler);
        void SetContentFolders(std::vector<std::string> const& folders);

        template<typename ...Args>
        constexpr void Route(Args&& ...args) 
        {
            HttpResponder::RouteHandler handler {std::forward<Args>(args)...};

            if (!handler.Target.empty())
                if (router_)
                    router_->RegisterRoute(handler.Target);

            if (!handler.Target.empty()
                || handler.Method.has_value()
                && handler.Callback.has_value())
                responder_->AddRouteHandler(handler);
        }

    protected:
        virtual void OnSocketError(Core::Error ec) override;
        virtual void OnSocketAccept(Core::Tcp::socket socket) override;
    };
}