#include "fl/web/HttpServer.hpp"
#include "fl/utils/Log.hpp"

namespace Forward::Web {

    HttpServer::HttpServer(uint32_t io_count)
        : SslServer(Core::SSL::Method::tlsv12_server, io_count)
    {
    }

    HttpServer::HttpServer(
        Core::SSL::Method method,
        uint32_t io_count
    )
        : SslServer(method, io_count)
    {
        responder_ = MakeRef<HttpResponder>();
    }

    HttpServer::HttpServer(
        std::string_view web_dir,
        Core::SSL::Method method, 
        uint32_t io_count
    )
        : SslServer(method, io_count)
    {
        router_ = MakeRef<HttpRouter>(web_dir);
        responder_ = MakeRef<HttpResponder>(router_);
    }

    HttpServer::~HttpServer()
    {
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
}