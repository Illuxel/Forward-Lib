#include "fl/net/http/HttpServer.hpp"
#include "fl/utils/Log.hpp"

namespace Forward::Web {

    HttpServer::HttpServer(
        Core::Ssl::context::method method, 
        uint32_t io_count)
        : SslServer(method, io_count)
    {
        responder_ = MakeRef<HttpResponder>();
    }

    HttpServer::HttpServer(
        std::string_view web_dir,
        Core::Ssl::context::method method, 
        uint32_t io_count)
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

    void HttpServer::OnSocketError(Core::Error ec)
    {

    }
    void HttpServer::OnSocketAccept(Core::Tcp::socket socket)
    {

    }
}