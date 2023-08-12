#pragma once

#include "fl/utils/Memory.hpp"

#include "fl/net/TcpServer.hpp"
#include "fl/net/SecureLayer.hpp"

namespace Forward::Net {
    /**
     * Secure TCP Server
     */
    class SslServer : public TcpServer, public SecureLayer
    {
    public:


    private:


    public:
        /**
         * Construct server with user defined amount of io threads
         * 
         * @param method sets encryption method of SSL connection.
         *                 By default it will use latest secure method
         * @param io_count specify amount of threads to io 
         */
        SslServer(Core::Ssl::context::method method, uint32_t io_count = 1);

        virtual ~SslServer() override;

    protected:
        virtual void OnSocketError(Core::Error ec);
        virtual void OnSocketAccept(Core::Tcp::socket socket);

    };
}
