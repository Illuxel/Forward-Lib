#include "fl/net/TcpServer.hpp"

#include "fl/utils/Log.hpp"

#include <boost/beast/core/bind_handler.hpp>

namespace Forward {

    TcpServer::TcpServer(uint8_t io_count)
        : io_context_(io_count)
        , acceptor_(io_context_)
    {
    }

    void TcpServer::Listen(Endpoint const& endpoint)
    {
        sys::error_code ec;
        Listen(endpoint, ec);
    }
    void TcpServer::Listen(Endpoint const& endpoint, sys::error_code& ec)
    {
        if (IsListening())
        {
            FL_LOG("Listener", "already listening");
            return;
        }

        // Open the acceptor
        acceptor_.open(endpoint.Protocol(), ec);

        if(ec)
            return;

        // Allow address reuse
        acceptor_.set_option(net::socket_base::reuse_address(true), ec);

        if(ec)
            return;

        // Bind to the server address
        acceptor_.bind(endpoint, ec);

        if(ec)
            return;

        // Start listening for connections
        acceptor_.listen(net::socket_base::max_listen_connections, ec);

        if(ec)
            return;

        is_listening = true;

        DoSocketAccept();
    }

    bool TcpServer::IsListening() const
    {
        return is_listening;
    }

    void TcpServer::OnSocketError(sys::error_code ec)
    {
    
    }
    void TcpServer::OnSocketAccept(tcp::socket socket)
    {

    }

    void TcpServer::DoSocketAccept()
    {
        acceptor_.async_accept(
            net::make_strand(io_context_),
            boost::beast::bind_front_handler(
                &TcpServer::HandleSocket,
                shared_from_this()));
    }
    void TcpServer::HandleSocket(sys::error_code ec, tcp::socket socket)
    {
        if (ec)
        {
            FL_LOG("TcpServer", ec);
            OnSocketError(ec);
            return;
        }

        OnSocketAccept(std::move(socket));

        DoSocketAccept();
    }
     
} // namespace Forward
