#pragma once

#include "fl/net/http/Core.hpp"

namespace Forward::Web {

    class HttpClient
    {
    private:
        Core::Tcp::resolver resolver_;
        Core::Beast::ssl_stream<Core::Beast::tcp_stream> stream_;

        Core::Beast::flat_buffer buffer_; // (Must persist between reads)

        Http::request<Http::empty_body> req_;
        Http::response<Http::string_body> res_;

    public:
        HttpClient(Core::Asio::any_io_executor ex, Core::Ssl::context& ctx);

        // Start the asynchronous operation
        void Run(std::string_view host, std::string_view port, std::string_view target);

        void OnResolve(Core::Error ec, Core::Tcp::resolver::results_type results);
        void OnConnect(Core::Error ec, Core::Tcp::resolver::results_type::endpoint_type);
        void OnHandshake(Core::Error ec);

        void OnWrite(Core::Error ec, std::size_t bytes_transferred);
        void OnRead(Core::Error ec, std::size_t bytes_transferred);
        
        void OnShutdown(Core::Error ec);
    };
}
