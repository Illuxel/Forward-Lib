#pragma once

#include "fl/utils/Log.hpp"

#include "fl/net/http/Core.hpp"

namespace fl {

    class Client : public std::enable_shared_from_this<Client>
    {
        tcp::resolver resolver_;
        beast::ssl_stream<beast::tcp_stream> stream_;
        beast::flat_buffer buffer_; // (Must persist between reads)
        http::request<http::empty_body> req_;
        http::response<http::string_body> res_;

    public:
        explicit Client(net::any_io_executor ex, ssl::context& ctx);

        // Start the asynchronous operation
        void Run( char const* host, char const* port,char const* target, int version);

        void OnResolve(beast::error_code ec, tcp::resolver::results_type results);
        void OnConnect(beast::error_code ec, tcp::resolver::results_type::endpoint_type);
        void OnHandshake(beast::error_code ec);

        void OnWrite(beast::error_code ec, std::size_t bytes_transferred);
        void OnRead(beast::error_code ec, std::size_t bytes_transferred);
        
        void OnShutdown(beast::error_code ec);
    };
}