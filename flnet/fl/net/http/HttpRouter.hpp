#pragma once

#include "fl/utils/Memory.hpp"

#include "fl/net/http/WebFileSystem.hpp"

namespace Forward::Web {

    struct RouteComparator
    {
        bool operator()(std::string_view left, std::string_view right) const 
        {
            return false;
        }
    };



    class HttpRouter
    {
    private:
        // default index name 
        std::string def_route_;
        // default extension for registered routes
        MimeType def_ext_;

        // file system to store every document info
        Scope<WebFilesSystem> wfs_;

        // folders where assets, styles, scripts stored
        std::set<std::string> content_;
        // key: reg route, value: prepared route
        std::map<std::string, std::string, RouteComparator> routes_;

        mutable std::shared_mutex router_mutex_;

    public:
        HttpRouter();
        HttpRouter(std::string_view web_root, std::string_view index = "index", std::string_view ext = "html");

        std::string GetPreparedTarget(std::string_view name) const;

        std::string GetRouteFilePath(std::string_view target) const;
        std::string GetContentFilePath(std::string_view content) const;

        void RegisterTarget(std::string_view target);
        void RegisterContent(std::string_view target);

        bool IsTarget(std::string_view target) const;
        bool IsTargetIndex(std::string_view target) const;

        bool IsContent(std::string_view content) const;

    private:
        // checks if a target is valid 
        // returns prepared target or index 
        std::string PrepareRouteName(std::string_view target) const;
    };
}