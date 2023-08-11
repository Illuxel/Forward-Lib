#pragma once

#include "fl/net/http/Core.hpp"

namespace Forward {

    class HttpSession : public std::enable_shared_from_this<HttpSession>
    {
    private:
        using ExpireTime = std::chrono::steady_clock::duration;

        beast::ssl_stream<beast::tcp_stream> stream_;
        ExpireTime exp_run_, exp_read_, exp_close_;
        
        beast::flat_buffer buffer_;

    protected:
        http::request<http::string_body> req_;

    public:
        explicit HttpSession(tcp::socket&& socket, net::ssl::context& context);
        virtual ~HttpSession();

        void SetRunExpire(ExpireTime exp_time);
        void SetReadExpire(ExpireTime exp_time);
        void SetCloseExpire(ExpireTime exp_time);

        // Start the session
        void Run();
        // Sends message to client
        void Write(http::message_generator&& msg);
        // Closes connection
        void Close();

    protected:
        /**
         *
         * void HttpSession::OnRead(sys::error_code ec, size_t bytes_transferred)
         * {
         *     boost::ignore_unused(bytes_transferred);
         * 
         *     if(ec == http::error::end_of_stream)
         *         return Close();
         * 
         *     if(ec)
         *         return FL_LOG("OnRead", ec.message());
         * 
         *     ... Your code
         *  
         *  }*/
        virtual void OnRead(sys::error_code ec, std::size_t bytes_transferred) = 0;

    private:
        void OnRun();
        void OnHandshake(sys::error_code ec);

        void Read();

        void OnWrite(bool keep_alive, sys::error_code ec, std::size_t bytes_transferred);
        void OnClose(sys::error_code ec);
    };
}