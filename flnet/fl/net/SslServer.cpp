#include "fl/net/SslServer.hpp"
#include "fl/utils/Log.hpp"

namespace Forward::Net {

    SslServer::SslServer(
        Core::Ssl::context::method method, 
        uint32_t io_count
    )
        : SecureLayer(method)
        , TcpServer(io_count)
    {
        //ssl::stream<tcp>
    }

    SslServer::~SslServer()
    {
    }

    void SslServer::OnSocketError(Core::Error ec)
    {

    }
    void SslServer::OnSocketAccept(Core::Tcp::socket socket)
    {

    }
}