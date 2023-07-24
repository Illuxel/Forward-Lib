#pragma once

#include "fl/net/SslServer.hpp"

#include "fl/net/http/HttpResponder.hpp"
#include "fl/net/http/HttpSession.hpp"

namespace Forward {

    class HttpServer : public SslServer
    {
    private:
        Ref<HttpRouter> router_;

    protected:
        Ref<HttpResponder> responder_;

    public:
        HttpServer(net::ssl::context::method method, uint8_t io_count);
        HttpServer(std::string_view web_dir, net::ssl::context::method method, uint8_t io_count);

        void SetBadRequest(HttpResponder::BadRequest const& handler);
        void SetContentFolders(std::vector<std::string> const& folders);

        template<typename ...Args>
        constexpr void Route(Args&& ...args) 
        {
            HttpResponder::HandlerData data {std::forward<Args>(args)...};

            if (!data.Target.empty())
                if (router_)
                    router_->RegisterRoute(data.Target);

            if (!data.Target.empty()
                || data.Method.has_value()
                && data.Callback.has_value())
                responder_->AddRouteHandler(data);
        }

    protected:
        virtual void OnSocketAccept(beast::error_code ec, tcp::socket&& socket) override;
    };
}