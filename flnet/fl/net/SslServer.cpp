#include "fl/net/SslServer.hpp"
#include "fl/utils/Log.hpp"

namespace Forward {

    SslServer::SslServer(ssl::context::method method, uint8_t io_count)
        : io_count_(io_count)
        , io_context_(io_count_)
        , secure_context_(method) 
    {
        io_sessions_.reserve(io_count_);
    }

    void SslServer::Listen(Endpoint const& endpoint)
    {
        if (listener_)
        {
            FL_LOG("Listener", "Already listening");
            return;
        }

        listener_ = MakeRef<AsyncListener>(
            io_context_, 
            endpoint, 
            beast::bind_front_handler(
                &SslServer::OnSocketAccept, this));

        if (!listener_->Listen())
        {
            FL_LOG("Listener", "Can not start listening port" + endpoint.Address() + ':' + std::to_string(endpoint.Port()));
            return;
        }

        for(auto i = 0; i < io_count_; ++i)
            io_sessions_.emplace_back([&]{
                io_context_.run();
            });

        io_context_.run();
    }

    void SslServer::SetupFileSslCert(std::string_view filename, 
                net::ssl::context::file_format format) 
    {
        boost::system::error_code ec;

        secure_context_.use_certificate_chain_file(filename.data(), ec);
        
        if (ec)
            FL_LOG("SslServer", ec.message());
    }
    void SslServer::SetupFileSslCertKey(std::string_view filename, std::string_view pass, 
                net::ssl::context::file_format format)
    {
        boost::system::error_code ec;

        if (!pass.empty())
        {
            secure_context_.set_password_callback([&](std::size_t, net::ssl::context::password_purpose){
                return pass.data();
            });
        }

        secure_context_.use_private_key_file(filename.data(), format, ec);

        if (ec)
            FL_LOG("SslServer", ec.message());
    }

    void SslServer::OnSocketAccept(beast::error_code ec, tcp::socket&& socket)
    {
        if (ec) {
            FL_LOG("SslServer", ec.message());
            return;
        }

        listener_->Accept();
    }
}