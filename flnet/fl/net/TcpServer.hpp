#pragma once

#include "fl/net/Endpoint.hpp"

namespace Forward::Net {

    /**
     * TCP Server that accepts sockets
     * 
     * Use with specified callbacks or just make derived class from TcpServer
     */
    class TcpServer
    {
    public:
        using OnErrorCallFunc = std::function<void(Core::ErrorCode)>;
        using OnSocketCallFunc = std::function<void(Core::TcpSocketBase)>;
        using OnSocketDataCallFunc = std::function<void(Core::TcpSocketBase&, Core::MutableBuffer const&)>;

    private:
        uint32_t io_count_;
        Core::IOContext io_context_;
        std::vector<std::thread> io_sessions_;

        Core::Tcp::acceptor acceptor_;

        bool is_listening = false,
            is_onerror = false,
            is_onsocket = false,
            is_onsocketdata = false;

        OnErrorCallFunc on_error_;

        OnSocketCallFunc on_socket_;
        OnSocketDataCallFunc on_socket_data_;

    public:
        TcpServer(uint32_t io_count = 1);

        virtual ~TcpServer();

        /**
         * Sets callback for socket error.
         * Emits when socket connection fails
         */
        void SetErrorCallback(OnErrorCallFunc const& callback);
        /**
         * Sets callback for a new connecting socket
         */
        void SetAcceptCallback(OnSocketCallFunc const& callback);
        /**
         * Sets callback for a new incoming socket data
         * 
         */
        void SetSocketDataCallback(OnSocketDataCallFunc const& callback);

        /**
         * Listens local host at port 8080
         */
        void Listen();
        /**
         * Listens local host at port 8080 with error return
         */
        void Listen(Core::ErrorCode& ec);

        void Listen(Endpoint const& endpoint);
        void Listen(Endpoint const& endpoint, Core::ErrorCode& ec);

        bool IsListening() const;

    protected:
        virtual void OnSocketError(Core::ErrorCode ec);
        virtual void OnSocketAccept(Core::TcpSocketBase socket);

        virtual void OnSocketData(Core::TcpSocketBase& socket, Core::MutableBuffer buffer);

    private:
        void StartIOContext();
        void AcceptNextSocket();
        void HandleSocket(Core::ErrorCode ec, Core::TcpSocketBase socket);

    };
} // namespace Forward
