#pragma once

#include "fl/utils/Memory.hpp"

#include "fl/net/TcpServer.hpp"
#include "fl/net/SecureLayer.hpp"

namespace Forward {
    /**
     * Secure TCP Server
     */
    class SslServer : public TcpServer, public SecureLayer
    {
    public:
        /**
         * Construct server with user defined amount of io threads
         * 
         * @param method sets encryption method of SSL connection.
         *                 By default it will use latest secure method
         * @param io_count specify amount of threads to io 
         */
        SslServer(ssl::context::method method, uint8_t io_count = 1);

        virtual ~SslServer() override;

    protected:
        virtual void OnSocketError(sys::error_code ec);
        virtual void OnSocketAccept(tcp::socket socket);

    };
}
