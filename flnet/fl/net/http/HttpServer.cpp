#include "fl/net/http/HttpServer.hpp"
#include "fl/utils/Log.hpp"

namespace fl {

    HttpServer::HttpServer(
        std::string_view web_dir,
        net::ssl::context::method method, 
        uint8_t io_count)
        : SslServer(method, io_count)
    {
        router_ = MakeRef<HttpRouter>(web_dir);
        responder_ = MakeRef<HttpResponder>(router_);
    }

    void HttpServer::SetContentFolders(std::vector<std::string> const& folders)
    {
        for (std::string_view folder : folders)
            router_->RegisterContent(folder);
    }

    void HttpServer::SetBadRequest(HttpResponder::BadRequest const& handler)
    {
        responder_->SetBadHandler(handler);
    }

    void HttpServer::OnSocketAccept(beast::error_code ec, tcp::socket&& socket)
    {
        MakeRef<HttpSession>(
            std::move(socket),
            secure_context_
        )->Run();

        SslServer::OnSocketAccept(ec, std::move(socket));
    }
}