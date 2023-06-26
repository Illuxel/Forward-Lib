#pragma once

#include "fl/net/http/HttpRequest.hpp"

namespace fl {

    class HttpSession : public std::enable_shared_from_this<HttpSession>
    {
        using ExpireTime = std::chrono::steady_clock::duration;

        beast::ssl_stream<beast::tcp_stream> stream_;
        beast::flat_buffer buffer_;

        ExpireTime exp_run_, exp_read_, exp_close_;

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
        virtual void OnRead(beast::error_code ec, std::size_t bytes_transferred);

    private:
        void OnRun();
        void OnHandshake(beast::error_code ec);

        void DoRead();

        void OnWrite(bool keep_alive, beast::error_code ec, std::size_t bytes_transferred);
        void OnClose(beast::error_code ec);
    };
}