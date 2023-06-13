#pragma once

#include "fl/utils/Memory.hpp"

#include "fl/net/http/WebFileSystem.hpp"

#include "fl/net/http/HttpRequest.hpp"
#include "fl/net/http/HttpResponse.hpp"

namespace fl {

    class HttpRouter
    {
    private:
        // 
        std::string index_;
        // default extension for registered routes
        MimeType def_ext_;

        // file system to store every document info
        Scope<WebFilesSystem const> wfs_;
        // folders where assets, styles, scripts stored
        std::unordered_set<std::string> content_folder_, routes_;

    public:
        HttpRouter(std::string_view web_root, std::string_view index = "index", std::string_view ext = "html");

        std::string GetRoutePath(std::string_view target) const;
        std::string GetContentPath(std::string_view content) const;

        void RegisterRoute(std::string_view target);
        void AddConentFolder(std::string_view folder_name);

        // checks if a target is valid 
        // returns prepared target or index 
        std::string MakeRouteFromName(std::string_view target) const;
        
        bool IsTarget(std::string_view target) const;
        bool IsContent(std::string_view content) const;

        static bool IsTargetValid(std::string_view target);

    private:
        bool IsTargetIndex(std::string_view target) const;
    };
}