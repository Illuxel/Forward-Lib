#include "fl/net/TcpServer.hpp"
#include "fl/core/Log.hpp"

#include <boost/beast/core/bind_handler.hpp>

namespace Forward::Net {

    TcpServer::TcpServer(uint32_t io_count)
        : io_count_(io_count)
        , io_context_(io_count)
        , acceptor_(io_context_)
    {
        io_sessions_.reserve(io_count);
    }

    TcpServer::~TcpServer()
    {
    
    }

    void TcpServer::Listen()
    {
        Core::ErrorCode ex;
        Listen(ex);
    }
    void TcpServer::Listen(Core::ErrorCode& ex)
    {
        Listen(Endpoint(), ex);
    }

    void TcpServer::Listen(Endpoint const& endpoint)
    {
        Core::ErrorCode ex;
        Listen(endpoint, ex);
    }
    void TcpServer::Listen(Endpoint const& endpoint, Core::ErrorCode& ex)
    {
        if (IsListening())
        {
            FL_LOG("Listener", "already listening");
            return;
        }

        // Open the acceptor
        acceptor_.open(endpoint.Protocol(), ex);

        if(ex)
            return;

        // Allow address reuse
        acceptor_.set_option(Core::TcpSocketBase::reuse_address(true));
        acceptor_.bind(endpoint, ex);

        if (ex) 
            return;

        acceptor_.listen(Core::TcpSocketBase::max_listen_connections, ex);

        if(ex)
            return;

        is_listening = true;

        AcceptNextSocket();
        StartIOContext();
    }

    bool TcpServer::IsListening() const
    {
        return is_listening;
    }

    void TcpServer::OnSocketError(Core::ErrorCode ex)
    {
        FL_LOG("TcpServer", ex);
    }
    void TcpServer::OnSocketAccept(Core::TcpSocketBase socket)
    {
        
    }

    void TcpServer::OnSocketData(Core::TcpSocketBase& socket, Core::MutableBuffer buffer)
    {
    
    }

    void TcpServer::StartIOContext() 
    {
        for (uint32_t i = 0; i < io_count_; ++i)
        {
            io_sessions_.emplace_back(
            [this]
            {
                io_context_.run();
            });
        }

        io_context_.run();
    }
    void TcpServer::AcceptNextSocket()
    {
        acceptor_.async_accept(
            Core::Asio::make_strand(io_context_),
            boost::beast::bind_front_handler(
                &TcpServer::HandleSocket, this));
    }
    void TcpServer::HandleSocket(Core::ErrorCode ex, Core::TcpSocketBase socket)
    {
        if (ex)
        {
            OnSocketError(ex);
            return;
        }
        else
        {
            OnSocketAccept(std::move(socket));
            AcceptNextSocket();
        }
    }
     
} // namespace Forward
