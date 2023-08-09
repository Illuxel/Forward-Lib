#include "fl/net/SslServer.hpp"
#include "fl/utils/Log.hpp"

namespace Forward {

    SslServer::SslServer(
        ssl::context::method method, 
        uint8_t io_count
    )
        : SecureLayer(method)
        , TcpServer(io_count)
    {

    }

    void SslServer::OnSocketError(sys::error_code ec)
    {

    }
    void SslServer::OnSocketAccept(tcp::socket socket)
    {

    }
}