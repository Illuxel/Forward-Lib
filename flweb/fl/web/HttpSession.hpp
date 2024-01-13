#pragma once

#include "fl/web/HttpRequest.hpp"

namespace Forward::Web {

    class HttpSession 
    {
    private:
        using ExpireTime = std::chrono::steady_clock::duration;

        Core::SSL::Stream<Core::TcpStream> ssl_stream_;
        ExpireTime exp_run_, exp_read_, exp_close_;
        
        Core::FlatBuffer buffer_;

    protected:
        HttpRequestString req_;

    public:
        explicit HttpSession(Core::Tcp::socket socket, Core::SSL::Context& context);
        virtual ~HttpSession();

        void SetRunExpire(ExpireTime exp_time);
        void SetReadExpire(ExpireTime exp_time);
        void SetCloseExpire(ExpireTime exp_time);

        // Start the session
        void Run();
        // Sends message to client
        void Write(Core::MessageGenerator&& msg);
        // Closes connection
        void Close();

    protected:
        /**
         *
         * void HttpSession::OnRead(sys::error_code ex, size_t bytes_transferred)
         * {
         *     boost::ignore_unused(bytes_transferred);
         * 
         *     if(ex == http::error::end_of_stream)
         *         return Close();
         * 
         *     if(ex)
         *         return FL_LOG("OnRead", ex.message());
         * 
         *     ... Your code
         *  
         *  }*/
        virtual void OnRead(Core::ErrorCode ex, std::size_t bytes_transferred) = 0;

    private:
        void OnRun();
        void OnHandshake(Core::ErrorCode ex);

        void Read();

        void OnWrite(bool keep_alive, Core::ErrorCode ex, std::size_t bytes_transferred);
        void OnClose(Core::ErrorCode ex);
    };
} // namespace Forward::Web