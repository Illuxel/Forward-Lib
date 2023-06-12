#pragma once

#include "fl/utils/Memory.hpp"

#include "fl/net/http/WebFilesSystem.hpp"

#include "fl/net/http/HttpRequest.hpp"
#include "fl/net/http/HttpResponse.hpp"

namespace fl {

    class HttpRouter
    {
    public: 
        using Callback = std::function<http::message_generator(HttpRequest const&, HttpResponse&&)>;

    private:
        // 
        std::string index_;
        // default extension for registered routes
        MimeType def_ext_;

        // folders where assets, styles, scripts stored
        std::list<std::string> content_folder_;
        // file system to store every document info
        Scope<WebFilesSystem const> wfs_;
        // registered routes that can be accessed
        std::map<std::string, std::optional<Callback>> routes_;

    public:
        HttpRouter(std::string_view web_root, std::string_view index = "index", std::string_view ext = "html");

        void AddConentFolder(std::string_view folder_name);

        void Register(std::string_view target);
        void Register(std::string_view target, std::optional<HttpRouter::Callback> const& handler);

        std::optional<Callback> 
        FindRouteCallback(std::string_view target) const;

        std::string GetTargetPath(std::string_view target) const;
        std::string GetContentPath(std::string_view content) const;
        
        bool IsTarget(std::string_view target) const;
        bool IsTargetIndex(std::string_view target) const;
        // checks if a target is valid 
        // returns prepared target or index 
        std::string MakeValidRoute(std::string_view target) const;

        static bool IsTargetValid(std::string_view target);

        bool IsContent(std::string_view content) const;
    };
}