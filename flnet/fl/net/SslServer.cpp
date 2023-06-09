#include "fl/net/SslServer.hpp"
#include "fl/utils/Log.hpp"

namespace fl {

    SslServer::SslServer(ssl::context::method method, uint8_t io_count)
        : io_count_(io_count)
        , io_context_(io_count_)
        , secure_context_(method) 
    {
        io_sessions_.reserve(io_count_);
    }

    void SslServer::Listen(Ref<Endpoint const> const& endpoint)
    {
        if (listener_)
        {
            FL_LOG("Listen", "Already listening");
            return;
        }

        listener_ = MakeRef<AsyncListener>(
            io_context_, 
            *endpoint, 
            beast::bind_front_handler(
                &SslServer::OnSocketAccept, this));

        if (!listener_->Listen())
        {
            FL_LOG("Listen", "Can not start listening port" + endpoint->Address() + ':' + std::to_string(endpoint->Port()));
            return;
        }

        for(auto i = 0; i < io_count_; ++i)
            io_sessions_.emplace_back([&]{
                io_context_.run();
            });

        FL_LOG("SslServer", "Started at " + endpoint->Address() + ':' + std::to_string(endpoint->Port()));

        io_context_.run();
    }

    void SslServer::SetupSslCert(std::string_view filename, 
                net::ssl::context::file_format format) 
    {
        boost::system::error_code ec;

        secure_context_.use_certificate_chain_file(filename.data(), ec);

        if (ec)
        {
            FL_LOG("Cert setup", ec.what());
        }
    }
    void SslServer::SetupSslCertKey(std::string_view filename, std::string_view pass, 
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
        {
            FL_LOG("Cert key setup", ec.what());
        }
    }

    void SslServer::OnSocketAccept(beast::error_code ec, tcp::socket&& socket)
    {
        if (ec) {
            FL_LOG("SslServer Accept", ec.what());
            return;
        }

        listener_->Accept();
    }
}