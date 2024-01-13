#include "fl/web/HttpServer.hpp"
#include "fl/core/Log.hpp"

namespace Forward::Web {

    HttpSession::HttpSession(
        Core::Tcp::socket socket, 
        Core::SSL::Context& context
    )
        : ssl_stream_(std::move(socket), context)
    {
        SetRunExpire(std::chrono::seconds(30));
        SetReadExpire(std::chrono::seconds(300));
        SetCloseExpire(std::chrono::seconds(30));
    }
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
        Core::Asio::dispatch(
            ssl_stream_.get_executor(),
            Core::Beast::bind_front_handler(
                &HttpSession::OnRun, this));
    }
    void HttpSession::OnRun()
    {
        Core::Beast::get_lowest_layer(ssl_stream_).expires_after(exp_run_);

        ssl_stream_.async_handshake(
            Core::SSL::StreamBase::server,
            Core::Beast::bind_front_handler(
                &HttpSession::OnHandshake, this));
    }
    void HttpSession::OnHandshake(Core::ErrorCode ex)
    {
        if(ex)
            return FL_LOG("handshake", ex.message());

        Read();
    }

    void HttpSession::Read()
    {
        req_ = {};

        Core::Beast::get_lowest_layer(ssl_stream_).expires_after(exp_read_);
        Core::Beast::async_read(ssl_stream_, buffer_, req_.Base(),
            Core::Beast::bind_front_handler(
                &HttpSession::OnRead, this));
    }

    void HttpSession::Write(Core::MessageGenerator&& msg)
    {
        bool keep_alive = msg.keep_alive();

        // Write the response
        Core::Beast::async_write(
            ssl_stream_,
            std::move(msg),
            Core::Beast::bind_front_handler(
                &HttpSession::OnWrite,
                this,
                keep_alive
            ));
    }
    void HttpSession::OnWrite(bool keep_alive, Core::ErrorCode ex, std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if(ex)
            return FL_LOG("OnWrite", ex.message());

        if(!keep_alive)
            return Close();

        Read();
    }

    void HttpSession::Close()
    {
        // Set the timeout.
        Core::Beast::get_lowest_layer(ssl_stream_).expires_after(exp_close_);

        // Perform the SSL shutdown
        ssl_stream_.async_shutdown(
            Core::Beast::bind_front_handler(
                &HttpSession::OnClose, this));
    }
    void HttpSession::OnClose(Core::ErrorCode ex)
    {
        if(ex)
            return FL_LOG("OnClose", ex);
    }
}