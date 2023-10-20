#pragma once

#include "fl/web/HttpRequest.hpp"
#include "fl/web/HttpResponse.hpp"

namespace Forward::Web {

    class HttpClient
    {
    private:
        Core::Tcp::resolver resolver_;
        Core::SSL::SslStream<Core::TcpStream> ssl_stream_;

        Core::FlatBuffer buffer_; // (Must persist between reads)

        HttpRequestEmpty req_;
        HttpResponseString res_;

    public:
        HttpClient(Core::AnyIOContext ex, Net::Core::SSL::Context& ctx);

        void Run(std::string_view host, std::string_view port, std::string_view target);

        void OnResolve(Core::ErrorCode ec, Core::TcpResolver::results_type results);
        void OnConnect(Core::ErrorCode ec, Core::TcpResolver::results_type::endpoint_type);
        void OnHandshake(Core::ErrorCode ec);

        void OnWrite(Core::ErrorCode ec, std::size_t bytes_transferred);
        void OnRead(Core::ErrorCode ec, std::size_t bytes_transferred);
        
        void OnShutdown(Core::ErrorCode ec);
    };
}
