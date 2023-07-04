#pragma once

#include "fl/net/http/WebFileSystem.hpp"

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
        std::set<std::string> content_folder_;
        // key: reg route, value: prepared route
        std::map<std::string, std::string> routes_;

    public:
        HttpRouter(std::string_view web_root, std::string_view index = "index", std::string_view ext = "html");

        std::string GetPreparedTarget(std::string_view name) const;

        std::string GetRoutePath(std::string_view target) const;
        std::string GetContentPath(std::string_view content) const;

        void RegisterRoute(std::string_view target);
        void RegisterContent(std::string_view folder_name);

        bool IsTarget(std::string_view target) const;
        bool IsTargetIndex(std::string_view target) const;

        bool IsContent(std::string_view content) const;

    private:
        // checks if a target is valid 
        // returns prepared target or index 
        std::string PrepareRouteName(std::string_view target) const;
    };
}