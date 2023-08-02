#pragma once

#include "fl/net/Endpoint.hpp"

namespace Forward {

    /**
     * 
     */
    class TcpServer : public std::enable_shared_from_this<TcpServer>
    {
    public:
        using OnAccept = std::function<void(sys::error_code, tcp::socket)>;

    private:
        tcp::acceptor acceptor_;
        net::io_context io_context_;

        Endpoint endpoint_;
        std::optional<OnAccept> callback_ = std::nullopt;

        bool is_listening = false;

    public:
        TcpServer();

        /**
         * 
         */
        TcpServer(uint8_t io_count);
        TcpServer(uint8_t io_count, Endpoint const& endpoint);

        /**
         *
         */
        TcpServer(Endpoint const& endpoint);
        TcpServer(Endpoint const& endpoint, OnAccept const& callback);

        /**
         * Sets callback which will be called after socket connection
         */
        void SetEndpoint(Endpoint const& endpoint);
        /**
         * Sets callback which will be called after socket connection
         */
        void SetCallback(OnAccept const& callback);

        net::io_context& GetContext();

        void Listen();

        bool IsListening() const;

    protected:
        virtual void Listen(sys::error_code& ec);
        virtual void OnSocketAccept(sys::error_code ec, tcp::socket socket);

    private:
        void DoSocketAccept();
        void HandleSocketError(sys::error_code ec, tcp::socket socket);
    };
} // namespace Forward