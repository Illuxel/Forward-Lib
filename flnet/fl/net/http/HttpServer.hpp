#pragma once

#include "fl/net/SslServer.hpp"

#include "fl/net/http/HttpResponder.hpp"
#include "fl/net/http/HttpSession.hpp"

namespace Forward {

    class HttpServer final : public SslServer
    {
    private:
        Ref<HttpRouter> router_;

    protected:
        Ref<HttpResponder> responder_;

    public:
        HttpServer(net::ssl::context::method method, uint8_t io_count = 1);
        HttpServer(std::string_view web_dir, net::ssl::context::method method, uint8_t io_count = 1);

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
        virtual void OnSocketError(sys::error_code ec) override;
        virtual void OnSocketAccept(tcp::socket socket) override;
    };
}