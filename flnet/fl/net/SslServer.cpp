#include "fl/net/SSLServer.hpp"
#include "fl/core/Log.hpp"

namespace Forward::Net {

    SslServer::SslServer(
        Core::SSL::Method method,
        uint32_t io_count
    )
        : TcpServer(io_count)
        , secure_context_(method)
    {
        //ssl::stream<tcp>
    }

    SslServer::~SslServer()
    {
    }

    void SslServer::SetupFileSSLCertKey(
        std::string_view filename,
        std::string_view pass,
        Core::SSL::FileFormat format)
    {
        Core::ErrorCode ex;

        if (!pass.empty())
        {
            secure_context_.set_password_callback(
                [&pass](uint64_t, Core::SSL::Context::password_purpose)
                {
                    return pass.data();
                }, ex);

            if (ex)
            {
                return FL_LOG("SslServer", ex);
            }
        }

        secure_context_.use_private_key_file(filename.data(), format, ex);

        if (ex)
        {
            FL_LOG("SslServer", ex);
        }
    }
}