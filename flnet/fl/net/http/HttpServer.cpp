#include "fl/net/http/HttpServer.hpp"
#include "fl/utils/Log.hpp"

namespace Forward {

    HttpServer::HttpServer(
        net::ssl::context::method method, 
        uint8_t io_count
    )
        : SslServer(method, io_count)
    {
        responder_ = MakeRef<HttpResponder>();
    }

    HttpServer::HttpServer(
        std::string_view web_dir,
        net::ssl::context::method method, 
        uint8_t io_count
    )
        : SslServer(method, io_count)
    {
        router_ = MakeRef<HttpRouter>(web_dir);
        responder_ = MakeRef<HttpResponder>(router_);
    }

    void HttpServer::SetBadRequest(HttpResponder::BadRequest const& handler)
    {
        responder_->SetBadHandler(handler);
    }
    void HttpServer::SetContentFolders(std::vector<std::string> const& folders)
    {
        if (!router_)
            return FL_LOG("HttpServer", "Router is not set");

        for (std::string_view folder : folders)
            router_->RegisterContent(folder);
    }

    void HttpServer::OnSocketError(sys::error_code ec)
    {

    }
    void HttpServer::OnSocketAccept(tcp::socket socket)
    {

    }
}