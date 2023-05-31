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
        // registered routes that can be accessed
        std::map<std::string, Callback> routes_;
        // file system to store every document info
        Ref<WebFilesSystem const> wfs_;

    public:
        /**
         *  @param web_root is a directory where web documents stored
         *  @param index    is an actual index file of page
         */
        HttpRouter(std::string_view web_root, std::string_view index = "index.html");

        void SetMainPage(std::string_view index);
        std::string_view GetMainPage() const;

        /**
         *  Register target in a system with function callback (handler)
         *  @param target   name to be registered
         *  @param handler  callback method
         */
        void Register(std::string_view target, Callback const& handler);
        /**
         *  @return callback method or null
         */
        std::optional<Callback> 
        FindRouteCallback(std::string_view target) const;

        /**
         *  @param target registered target name
         *  @return target with path. if target invalid will return index target
         */
        std::string GetTargetPath(std::string_view target) const;

        /**
         *  @param target registered target name
         *  @return true if target is index
         */
        bool IsTargetIndex(std::string_view target) const;
        /**
         *  Checks if target is routed and file exist 
         * 
         *  @param target registered target name
         */
        bool IsTargetRegisted(std::string_view target) const;

        /**
         *  Checks if target is routed and file exist 
         * 
         *  @param target registered target name
         */
        bool IsContentTarget(std::string_view target) const;

        /**
         *  Check if the target has corresponding format
         * 
         *  @return target path in system file system
         */
        static bool IsTargetLegal(std::string_view target);

    private:
        // checks if a target is valid 
        // returns prepared target or index 
        std::string PrepareRoute(std::string_view target) const;

    };
}