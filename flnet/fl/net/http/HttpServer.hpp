#pragma once

#include "fl/net/SslServer.hpp"

#include "fl/net/http/HttpResponder.hpp"
#include "fl/net/http/HttpSession.hpp"

namespace fl {

    class HttpServer : public SslServer
    {
    private:
        Ref<HttpRouter> router_;
        Ref<HttpResponder> responder_;

        std::list<Ref<HttpSession>> user_sessions_;

    public:
        HttpServer(std::string_view web_dir, net::ssl::context::method method, uint8_t io_count);

        void SetContentFolders(std::vector<std::string> const& folders);
        void SetBadRequest(HttpResponder::BadRequest const& handler);

        template<typename ...Args>
        void Route(Args&& ...args) 
        {
            HttpResponder::HandlerData data {std::forward<Args>(args)...};

            if (!data.Target.empty())
                router_->RegisterRoute(data.Target);

            if (data.Callback.has_value() || data.Method.has_value())
                responder_->AddRouteHandler(data);
        }

    protected:
        virtual void OnSocketAccept(beast::error_code ec, tcp::socket&& socket) override;
    };
}