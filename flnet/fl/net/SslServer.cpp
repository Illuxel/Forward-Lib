#include "fl/net/SslServer.hpp"
#include "fl/utils/Log.hpp"

namespace Forward {

    SslServer::SslServer(ssl::context::method method)
        : SecureLayer(method)
        , TcpServer()
    {
    }

    SslServer::SslServer(ssl::context::method method, uint8_t io_count)
        : SecureLayer(method)
        , TcpServer()
        , io_count_(io_count)
    {
        io_sessions_.reserve(io_count_);
    }

    void SslServer::Listen(sys::error_code& ec)
    {
        TcpServer::Listen(ec);

        for (auto i = 0; i < io_count_; ++i)
        {
            io_sessions_.emplace_back(
            [this]() {
                this->GetContext().run();
            });
        }

        this->GetContext().run();
    }

    void SslServer::OnSocketAccept(sys::error_code ec, tcp::socket socket)
    {

    }
}