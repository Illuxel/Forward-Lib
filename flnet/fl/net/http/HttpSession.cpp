#include "fl/net/http/HttpServer.hpp"
#include "fl/utils/Log.hpp"

namespace fl {

    HttpSession::HttpSession(tcp::socket&& socket, net::ssl::context& context)
        : stream_(std::move(socket), context) {}
        
    HttpSession::~HttpSession() {}
        
    void HttpSession::SetRunExpire(ExpireTime exp_time)
    {
        exp_run_ = exp_time;
    }
    void HttpSession::SetReadExpire(ExpireTime exp_time)
    {
        exp_read_ = exp_time;
    }
    void HttpSession::SetCloseExpire(ExpireTime exp_time)
    {
        exp_close_ = exp_time;
    }

    void HttpSession::Run()
    {
        net::dispatch(
            stream_.get_executor(),
            beast::bind_front_handler(
                &HttpSession::OnRun,
                this->shared_from_this()));
    }
    void HttpSession::OnRun()
    {
        beast::get_lowest_layer(stream_).expires_after(exp_run_);

        // Perform the SSL handshake
        stream_.async_handshake(
            net::ssl::stream_base::server,
            beast::bind_front_handler(
                &HttpSession::OnHandshake,
                shared_from_this()));
    }
    void HttpSession::OnHandshake(beast::error_code ec) 
    {
        if(ec)
            return FL_LOG("handshake", ec.message());

        DoRead();
    }

    void HttpSession::DoRead()
    {
        req_ = {};

        // Set the timeout.
        beast::get_lowest_layer(stream_).expires_after(exp_read_);

        // Read a request using the parser-oriented interface
        http::async_read(stream_, buffer_, req_,
            beast::bind_front_handler(
                &HttpSession::OnRead,
                shared_from_this()));
    }
    void HttpSession::OnRead(beast::error_code ec, size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        // This means they closed the connection
        if(ec == http::error::end_of_stream)
            return DoClose();

        if(ec)
            return FL_LOG("OnRead", ec.message());

        // Send the response
        // DoWrite(responder_->HandleRequest(std::move(req_)));
    }

    void HttpSession::DoWrite(http::message_generator&& msg)
    {
        bool keep_alive = msg.keep_alive();

        // Write the response
        beast::async_write(
            stream_,
            std::move(msg),
            beast::bind_front_handler(
                &HttpSession::OnWrite,
                this->shared_from_this(),
                keep_alive
            ));
    }
    void HttpSession::OnWrite(bool keep_alive, beast::error_code ec, std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return FL_LOG("OnWrite", ec.message());

        if(!keep_alive)
            return DoClose();

        DoRead();
    }

    void HttpSession::DoClose()
    {
        // Set the timeout.
        beast::get_lowest_layer(stream_).expires_after(exp_close_);

        // Perform the SSL shutdown
        stream_.async_shutdown(
            beast::bind_front_handler(
                &HttpSession::OnClose,
                shared_from_this()));
    }
    void HttpSession::OnClose(beast::error_code ec)
    {
        if(ec)
            return FL_LOG("OnClose", ec.message());
    }
}