#pragma once

#include "fl/utils/Memory.hpp"

#include "fl/net/AsyncListener.hpp"

namespace fl {

    class SslServer 
    {
    protected:
        size_t io_count_;
        net::io_context io_context_;
        std::vector<std::thread> io_sessions_;

        ssl::context secure_context_;

        Ref<AsyncListener> listener_;

    public:
        /**
         *  @param method   sets encription method of SSL connection.
         *                  by default it will use latest secure method
         * 
         *  @param io_count specify amount of threads to io 
         */
        SslServer(ssl::context::method method, uint8_t io_count);

        void Listen(Ref<Endpoint const> const& endpoint);

        void SetupSslCert(std::string_view filename, 
            net::ssl::context::file_format format = net::ssl::context::pem);
        void SetupSslCertKey(std::string_view filename, std::string_view pass = "",
            net::ssl::context::file_format format = net::ssl::context::pem);

        virtual void OnSocketAccept(beast::error_code ec, tcp::socket&& socket) = 0;
    };

}