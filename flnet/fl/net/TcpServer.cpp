#include "fl/net/TcpServer.hpp"
#include "fl/utils/Log.hpp"

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

    void TcpServer::SetErrorCallback(OnErrorCallFunc const& callback)
    {
        on_error_ = callback;
        is_onerror = true;
    }
    void TcpServer::SetAcceptCallback(OnSocketCallFunc const& callback)
    {
        on_socket_ = callback;
        is_onsocket = true;
    }

    void TcpServer::SetSocketDataCallback(OnSocketDataCallFunc const& callback) 
    {
        on_socket_data_ = callback;
        is_onsocketdata = true;
    }

    void TcpServer::Listen()
    {
        Core::ErrorCode ec;
        Listen(ec);
    }
    void TcpServer::Listen(Core::ErrorCode& ec)
    {
        Listen(Endpoint(), ec);
    }

    void TcpServer::Listen(Endpoint const& endpoint)
    {
        Core::ErrorCode ec;
        Listen(endpoint, ec);
    }
    void TcpServer::Listen(Endpoint const& endpoint, Core::ErrorCode& ec)
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
        acceptor_.set_option(Core::TcpSocketBase::reuse_address(true));
        acceptor_.bind(endpoint, ec);

        if (ec) 
            return;

        acceptor_.listen(Core::TcpSocketBase::max_listen_connections, ec);

        if(ec)
            return;

        is_listening = true;

        AcceptNextSocket();
        StartIOContext();
    }

    bool TcpServer::IsListening() const
    {
        return is_listening;
    }

    void TcpServer::OnSocketError(Core::ErrorCode ec)
    {
        FL_LOG("TcpServer", ec);
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
    void TcpServer::HandleSocket(Core::ErrorCode ec, Core::TcpSocketBase socket)
    {
        if (ec)
        {
            OnSocketError(ec);

            if (is_onerror)
                on_error_(ec);

            return;
        }

        if (is_onsocket)
            on_socket_(std::move(socket));
        else
            OnSocketAccept(std::move(socket));

        AcceptNextSocket();
    }
     
} // namespace Forward
