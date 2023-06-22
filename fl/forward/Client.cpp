#include "fl/forward/Client.hpp"

namespace fl {

    Client::Client(net::any_io_executor ex, ssl::context& ctx)
        : resolver_(ex)
        , stream_(ex, ctx)
    {
    }

    // Start the asynchronous operation
    void Client::Run( char const* host, char const* port,char const* target, int version)
    {
        // Set SNI Hostname (many hosts need this to handshake successfully)
        if(! SSL_set_tlsext_host_name(stream_.native_handle(), host))
        {
            beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
            // std::cerr << ec.message() << "\n";
            return;
        }

        // Set up an HTTP GET request message
        req_.version(version);
        req_.method(http::verb::get);
        req_.target(target);
        req_.set(http::field::host, host);
        req_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        // Look up the domain name
        resolver_.async_resolve(
            host,
            port,
            beast::bind_front_handler(
                &Client::OnResolve,
                shared_from_this()));
    }

    void Client::OnResolve(beast::error_code ec, tcp::resolver::results_type results)
    {
        if(ec)
            return FL_LOG("resolve", ec.message());

        // Set a timeout on the operation
        beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

        // Make the connection on the IP address we get from a lookup
        beast::get_lowest_layer(stream_).async_connect(
            results,
            beast::bind_front_handler(
                &Client::OnConnect,
                shared_from_this()));
    }

    void Client::OnConnect(beast::error_code ec, tcp::resolver::results_type::endpoint_type)
    {
        if(ec)
            return FL_LOG("connect", ec.message());

        // Perform the SSL handshake
        stream_.async_handshake(
            ssl::stream_base::client,
            beast::bind_front_handler(
                &Client::OnHandshake,
                shared_from_this()));
    }

    void Client::OnHandshake(beast::error_code ec)
    {
        if(ec)
            return FL_LOG("handshake", ec.message());

        // Set a timeout on the operation
        beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

        // Send the HTTP request to the remote host
        http::async_write(stream_, req_,
            beast::bind_front_handler(
                &Client::OnWrite,
                shared_from_this()));
    }

    void Client::OnWrite(beast::error_code ec, std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return FL_LOG("write", ec.message());

        // Receive the HTTP response
        http::async_read(stream_, buffer_, res_,
            beast::bind_front_handler(
                &Client::OnRead,
                shared_from_this()));
    }

    void Client::OnRead(beast::error_code ec, std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return FL_LOG("read", ec.message());

        // Write the message to standard out
        FL_LOG("read", res_.body());

        // Set a timeout on the operation
        beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

        // Gracefully close the stream
        stream_.async_shutdown(
            beast::bind_front_handler(
                &Client::OnShutdown,
                shared_from_this()));
    }

    void Client::OnShutdown(beast::error_code ec)
    {
        if(ec == net::error::eof)
        {
            // Rationale:
            // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
            ec = {};
        }
        if(ec)
            return FL_LOG("shutdown", ec.message());

        // If we get here then the connection is closed gracefully
    }
}