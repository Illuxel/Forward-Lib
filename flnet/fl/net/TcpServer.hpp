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
        using OnError = std::function<void(Core::Error)>;
        using OnSocket = std::function<void(Core::Tcp::socket)>;

    private:
        uint32_t io_count_;
        Core::Asio::io_context io_context_;
        std::vector<std::thread> io_sessions_;

        Core::Tcp::acceptor acceptor_;

        bool is_listening = false, 
            is_onerror = false, 
            is_onsocket = false;

        OnError on_error_;
        OnSocket on_socket_;

    public:
        TcpServer(uint32_t io_count = 1);

        virtual ~TcpServer();

        /**
         * Sets callback for socket error.
         * Emits when socket connection fails
         */
        void SetErrorCallback(OnError const& callback);
        /**
         * Sets callback for a new connecting socket
         */
        void SetAcceptCallback(OnSocket const& callback);

        /**
         * Listens local host at port 8080
         */
        void Listen();
        /**
         * Listens local host at port 8080 with error return
         */
        void Listen(Core::Error& ec);

        void Listen(Endpoint const& endpoint);
        void Listen(Endpoint const& endpoint, Core::Error& ec);

        bool IsListening() const;

    protected:
        virtual void OnSocketError(Core::Error ec);
        virtual void OnSocketAccept(Core::Tcp::socket socket);

    private:
        void StartIOContext();
        void AcceptNextSocket();
        void HandleSocket(Core::Error ec, Core::Tcp::socket socket);

    };
} // namespace Forward
