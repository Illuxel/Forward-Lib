#include "fl/web/HttpClient.hpp"
#include "fl/utils/Log.hpp"

namespace Forward::Web {

    HttpClient::HttpClient(
        Core::AnyIOContext ex, 
        Net::Core::SSL::Context& ctx 
    )
        : resolver_(ex)
        , ssl_stream_(ex, ctx)
    {
    }

    // Start the asynchronous operation
    void HttpClient::Run(
        std::string_view host, 
        std::string_view port, 
        std::string_view target)
    {
        // Set SNI Hostname (many hosts need this to handshake successfully)
        if(!SSL_set_tlsext_host_name(ssl_stream_.native_handle(), host.data()))
        {
            Core::ErrorCode ec{static_cast<int>(::ERR_get_error()), Core::Error::get_ssl_category()};
            return FL_LOG("HttpClient", ec);
        }

        // Set up an HTTP GET request message
        req_.Base().version(11);
        req_.Base().method(HttpMethod::get);
        req_.Base().target(target);
        req_.Base().set(HttpField::host, host);
        req_.Base().set(HttpField::user_agent, BOOST_BEAST_VERSION_STRING);

        // Look up the domain name
        resolver_.async_resolve(
            host,
            port,
            Core::Beast::bind_front_handler(
                &HttpClient::OnResolve, this));
    }

    void HttpClient::OnResolve(
        Core::ErrorCode ec, 
        Core::TcpResolver::results_type results)
    {
        if(ec)
            return FL_LOG("resolve", ec);

        // Set a timeout on the operation
        Core::Beast::get_lowest_layer(ssl_stream_).expires_after(std::chrono::seconds(310));

        // Make the connection on the IP address we get from a lookup
        Core::Beast::get_lowest_layer(ssl_stream_).async_connect(
            results,
            Core::Beast::bind_front_handler(
                &HttpClient::OnConnect, this));
    }

    void HttpClient::OnConnect(
        Core::ErrorCode ec,
        Core::TcpResolver::endpoint_type)
    {
        if(ec)
            return FL_LOG("connect", ec);

        // Perform the SSL handshake
        ssl_stream_.async_handshake(
            Core::SSL::StreamBase::client,
            Core::Beast::bind_front_handler(
                &HttpClient::OnHandshake,
                this));
    }

    void HttpClient::OnHandshake(Core::ErrorCode ec)
    {
        if(ec)
            return FL_LOG("handshake", ec);

        // Set a timeout on the operation
        Core::Beast::get_lowest_layer(ssl_stream_).expires_after(std::chrono::seconds(310));

        // Send the HTTP request to the remote host
        Core::Beast::async_write(ssl_stream_, req_.Base(),
            Core::Beast::bind_front_handler(
                &HttpClient::OnWrite, this));
    }

    void HttpClient::OnWrite(
        Core::ErrorCode ec, 
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return FL_LOG("write", ec);

        // Receive the HTTP response
        Core::Beast::async_read(ssl_stream_, buffer_, res_.Base(),
            Core::Beast::bind_front_handler(
                &HttpClient::OnRead, this));
    }

    void HttpClient::OnRead(Core::ErrorCode ec, std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return FL_LOG("read", ec);

        // Write the message to standard out
        FL_LOG("read", res_.Base().body());

        // Set a timeout on the operation
        Core::Beast::get_lowest_layer(ssl_stream_).expires_after(std::chrono::seconds(310));

        // Gracefully close the stream
        ssl_stream_.async_shutdown(
            Core::Beast::bind_front_handler(
                &HttpClient::OnShutdown, this));
    }

    void HttpClient::OnShutdown(Core::ErrorCode ec)
    {
        if(ec == Core::Error::eof)
        {
            // Rationale:
            // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
            ec = {};
        }
        if(ec)
            return FL_LOG("shutdown", ec);

        // If we get here then the connection is closed gracefully
    }
}