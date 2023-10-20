#pragma once

#include "fl/net/TcpServer.hpp"

namespace Forward::Net {
    /**
     * Secure TCP Server
     */
    class SslServer : public TcpServer
    {
    public:


    private:
        Core::SSL::Context secure_context_;

    public:
        /**
         * Construct server with user defined amount of io threads
         * 
         * @param method sets encryption method of SSL connection.
         *                 By default it will use latest secure method
         * @param io_count specify amount of threads to io 
         */
        SslServer(Core::SSL::Method method = Core::SSL::Method::tlsv13_server, uint32_t io_count = 1);

        virtual ~SslServer() override;

        void SetupFileSSLCertKey(std::string_view filename, std::string_view pass = "",
            Core::SSL::FileFormat format = Core::SSL::FileFormat::pem);
    };
}
