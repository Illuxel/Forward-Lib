#include "fl/net/TcpServer.hpp"

#include "fl/utils/Log.hpp"

#include <boost/beast/core/bind_handler.hpp>

namespace Forward {

    TcpServer::TcpServer()
        : acceptor_(io_context_)
    {
    }

    TcpServer::TcpServer(uint8_t io_count)
        : io_context_(io_count)
        , acceptor_(io_context_)
    {
    }
    TcpServer::TcpServer(uint8_t io_count, Endpoint const& endpoint)
        : endpoint_(endpoint)
        , io_context_(io_count)
        , acceptor_(io_context_)
    {
    }

    TcpServer::TcpServer(Endpoint const& endpoint)
        : endpoint_(endpoint)
        , acceptor_(io_context_)
    {
    }
    TcpServer::TcpServer(Endpoint const& endpoint, OnAccept const& callback)
        : endpoint_(endpoint) 
        , acceptor_(io_context_)
    {
        SetCallback(callback);
    }

    void TcpServer::SetEndpoint(Endpoint const& endpoint)
    {
        endpoint_ = endpoint;
    }
    void TcpServer::SetCallback(OnAccept const& callback)
    {
        callback_.emplace(callback);
    }

    net::io_context& TcpServer::GetContext()
    {
        return io_context_;
    }

    void TcpServer::Listen()
    {
        sys::error_code ec;

        Listen(ec);
        FL_LOG("Listener", ec);
    }
    void TcpServer::Listen(sys::error_code& ec)
    {
        if (IsListening())
        {
            FL_LOG("Listener", "Already listening");
            return;
        }

        // Open the acceptor
        acceptor_.open(endpoint_.Protocol(), ec);

        if(ec)
            return;

        // Allow address reuse
        acceptor_.set_option(net::socket_base::reuse_address(true), ec);

        if(ec)
            return;

        // Bind to the server address
        acceptor_.bind(endpoint_, ec);

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

    void TcpServer::OnSocketAccept(sys::error_code ec, tcp::socket socket)
    {

    }

    void TcpServer::DoSocketAccept()
    {
        acceptor_.async_accept(
            net::make_strand(io_context_),
            boost::beast::bind_front_handler(
                &TcpServer::CheckSocketError,
                shared_from_this()));
    }
    void TcpServer::CheckSocketError(sys::error_code ec, tcp::socket socket)
    {
        if (ec)
        {
            FL_LOG("TcpServer", ec);
            return;
        }

        if (callback_.has_value())
            callback_.value()(ec, std::move(socket));
        else 
            OnSocketAccept(ec, std::move(socket));

        DoSocketAccept();
    }
     
} // namespace Forward
