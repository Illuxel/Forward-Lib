#include "fl/net/http/HttpServer.hpp"
#include "fl/utils/Log.hpp"

namespace Forward::Web {

    HttpSession::HttpSession(Core::Tcp::socket socket, Core::Ssl::context& context)
        : stream_(std::move(socket), context) 
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
            stream_.get_executor(),
            Core::Beast::bind_front_handler(
                &HttpSession::OnRun, this));
    }
    void HttpSession::OnRun()
    {
        Core::Beast::get_lowest_layer(stream_).expires_after(exp_run_);

        stream_.async_handshake(
            Core::Asio::ssl::stream_base::server,
            Core::Beast::bind_front_handler(
                &HttpSession::OnHandshake, this));
    }
    void HttpSession::OnHandshake(Core::Error ec)
    {
        if(ec)
            return FL_LOG("handshake", ec.message());

        Read();
    }

    void HttpSession::Read()
    {
        req_ = {};

        Core::Beast::get_lowest_layer(stream_).expires_after(exp_read_);

        Http::async_read(stream_, buffer_, req_,
            Core::Beast::bind_front_handler(
                &HttpSession::OnRead, this));
    }

    void HttpSession::Write(Http::message_generator&& msg)
    {
        bool keep_alive = msg.keep_alive();

        // Write the response
        Core::Beast::async_write(
            stream_,
            std::move(msg),
            Core::Beast::bind_front_handler(
                &HttpSession::OnWrite,
                this,
                keep_alive
            ));
    }
    void HttpSession::OnWrite(bool keep_alive, Core::Error ec, std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return FL_LOG("OnWrite", ec.message());

        if(!keep_alive)
            return Close();

        Read();
    }

    void HttpSession::Close()
    {
        // Set the timeout.
        Core::Beast::get_lowest_layer(stream_).expires_after(exp_close_);

        // Perform the SSL shutdown
        stream_.async_shutdown(
            Core::Beast::bind_front_handler(
                &HttpSession::OnClose, this));
    }
    void HttpSession::OnClose(Core::Error ec)
    {
        if(ec)
            return FL_LOG("OnClose", ec);
    }
}