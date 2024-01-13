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
    private:
        uint32_t io_count_;
        Core::IOContext io_context_;
        std::vector<std::thread> io_sessions_;

        Core::Tcp::acceptor acceptor_;

        bool is_listening = false;

    public:
        TcpServer(uint32_t io_count = 1);

        virtual ~TcpServer();

        /**
         * Listens local host at port 8080
         */
        void Listen();
        /**
         * Listens local host at port 8080 with error return
         */
        void Listen(Core::ErrorCode& ex);

        void Listen(Endpoint const& endpoint);
        void Listen(Endpoint const& endpoint, Core::ErrorCode& ex);

        bool IsListening() const;

    protected:
        virtual void OnSocketError(Core::ErrorCode ex);
        virtual void OnSocketAccept(Core::TcpSocketBase socket);

        virtual void OnSocketData(Core::TcpSocketBase& socket, Core::MutableBuffer buffer);

    private:
        void StartIOContext();
        void AcceptNextSocket();
        void HandleSocket(Core::ErrorCode ex, Core::TcpSocketBase socket);

    };
} // namespace Forward
