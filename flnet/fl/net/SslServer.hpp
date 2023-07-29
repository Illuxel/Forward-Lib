#pragma once

#include "fl/utils/Memory.hpp"

#include "fl/net/TcpServer.hpp"
#include "fl/net/SecureLayer.hpp"

namespace Forward {

    class SslServer : public TcpServer, public SecureLayer
    {
    private:
        uint8_t io_count_;
        std::vector<std::thread> io_sessions_;

    public:
        /**
         * Construct server in single thread mode
         * 
         * @param method   sets encryption method of SSL connection.
         *                  by default it will use latest secure method
         * @param io_count specify amount of threads to io
         */
        SslServer(ssl::context::method method);
        /**
         * Construct server with user defined amount of io threads
         * 
         * @param method   sets encryption method of SSL connection.
         *                  by default it will use latest secure method
         * @param io_count specify amount of threads to io 
         */
        SslServer(ssl::context::method method, uint8_t io_count);

        void Listen(sys::error_code& ec) override;

    protected:
        virtual void OnSocketAccept(sys::error_code ec, tcp::socket socket);

    };
}
