#include "fl/net/http/HttpClient.hpp"
#include "fl/utils/Log.hpp"


namespace Forward::Web {

    HttpClient::HttpClient(
        Core::Asio::any_io_executor ex, 
        Core::Ssl::context& ctx)
        : resolver_(ex)
        , stream_(ex, ctx)
    {
    }

    // Start the asynchronous operation
    void HttpClient::Run(
        std::string_view host, 
        std::string_view port, 
        std::string_view target)
    {
        // Set SNI Hostname (many hosts need this to handshake successfully)
        if(! SSL_set_tlsext_host_name(stream_.native_handle(), host.data()))
        {
            Core::Error ec{static_cast<int>(::ERR_get_error()), Core::Asio::error::get_ssl_category()};
            return FL_LOG("HttpClient", ec);
        }

        // Set up an HTTP GET request message
        req_.version(11);
        req_.method(Http::verb::get);
        req_.target(target);
        req_.set(Http::field::host, host);
        req_.set(Http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        // Look up the domain name
        resolver_.async_resolve(
            host,
            port,
            Core::Beast::bind_front_handler(
                &HttpClient::OnResolve, this));
    }

    void HttpClient::OnResolve(
        Core::Error ec, 
        Core::Tcp::resolver::results_type results)
    {
        if(ec)
            return FL_LOG("resolve", ec);

        // Set a timeout on the operation
        Core::Beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(310));

        // Make the connection on the IP address we get from a lookup
        Core::Beast::get_lowest_layer(stream_).async_connect(
            results,
            Core::Beast::bind_front_handler(
                &HttpClient::OnConnect, this));
    }

    void HttpClient::OnConnect(
        Core::Error ec, 
        Core::Tcp::resolver::results_type::endpoint_type)
    {
        if(ec)
            return FL_LOG("connect", ec);

        // Perform the SSL handshake
        stream_.async_handshake(
            Core::Ssl::stream_base::client,
            Core::Beast::bind_front_handler(
                &HttpClient::OnHandshake,
                this));
    }

    void HttpClient::OnHandshake(Core::Error ec)
    {
        if(ec)
            return FL_LOG("handshake", ec);

        // Set a timeout on the operation
        Core::Beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(310));

        // Send the HTTP request to the remote host
        Http::async_write(stream_, req_,
            Core::Beast::bind_front_handler(
                &HttpClient::OnWrite, this));
    }

    void HttpClient::OnWrite(Core::Error ec, std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return FL_LOG("write", ec);

        // Receive the HTTP response
        Http::async_read(stream_, buffer_, res_,
            Core::Beast::bind_front_handler(
                &HttpClient::OnRead, this));
    }

    void HttpClient::OnRead(Core::Error ec, std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return FL_LOG("read", ec);

        // Write the message to standard out
        FL_LOG("read", res_.body());

        // Set a timeout on the operation
        Core::Beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(310));

        // Gracefully close the stream
        stream_.async_shutdown(
            Core::Beast::bind_front_handler(
                &HttpClient::OnShutdown, this));
    }

    void HttpClient::OnShutdown(Core::Error ec)
    {
        if(ec == Core::Asio::error::eof)
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