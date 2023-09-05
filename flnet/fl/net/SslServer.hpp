#pragma once

#include "fl/utils/Memory.hpp"

#include "fl/net/TcpServer.hpp"

namespace Forward::Net {
    /**
     * Secure TCP Server
     */
    class SslServer : public TcpServer
    {
    public:


    private:
        Core::SSL::context secure_context_;

    public:
        /**
         * Construct server with user defined amount of io threads
         * 
         * @param method sets encryption method of SSL connection.
         *                 By default it will use latest secure method
         * @param io_count specify amount of threads to io 
         */
        SslServer(Core::ssl::context::method method, uint32_t io_count = 1);

        virtual ~SslServer() override;

        void SetupFileSSLCert(std::string_view filename,
            Core::SSL::context::file_format format = Core::SSL::context::pem);
        void SetupFileSSLCertKey(std::string_view filename, std::string_view pass = "",
            Core::SSL::context::file_format format = Core::SSL::context::pem);
    };
}
