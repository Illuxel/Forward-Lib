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
        HttpClient(Core::any_io_executor ex, Core::SSL::context& ctx);

        // Start the asynchronous operation
        void Run(std::string_view host, std::string_view port, std::string_view target);

        void OnResolve(Core::ErrorCode ec, Core::Tcp::resolver::results_type results);
        void OnConnect(Core::ErrorCode ec, Core::Tcp::resolver::results_type::endpoint_type);
        void OnHandshake(Core::ErrorCode ec);

        void OnWrite(Core::ErrorCode ec, std::size_t bytes_transferred);
        void OnRead(Core::ErrorCode ec, std::size_t bytes_transferred);
        
        void OnShutdown(Core::ErrorCode ec);
    };
}
