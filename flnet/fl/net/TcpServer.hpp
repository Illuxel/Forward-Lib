#pragma once

#include "fl/net/Endpoint.hpp"

namespace Forward {

    /**
     * TCP Server that accepts sockets
     */
    class TcpServer : public std::enable_shared_from_this<TcpServer>
    {
    private:
        net::io_context io_context_;
        std::vector<std::thread> io_sessions_;

        tcp::acceptor acceptor_;

        bool is_listening = false;

    public:
        TcpServer(uint8_t io_count = 1);
        virtual ~TcpServer();

        void Listen(Endpoint const& endpoint);
        void Listen(Endpoint const& endpoint, sys::error_code& ec);

        bool IsListening() const;

    protected:
        virtual void OnSocketError(sys::error_code ec);
        virtual void OnSocketAccept(tcp::socket socket);

    private:
        void HandleSocket(sys::error_code ec, tcp::socket socket);
        void AcceptNextSocket();

    };
} // namespace Forward