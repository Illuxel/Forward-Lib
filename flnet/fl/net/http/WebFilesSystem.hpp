#pragma once

#include "fl/utils/MimeType.hpp"

#include "fl/net/Core.hpp"

#include <map>
#include <optional>
#include <filesystem>

namespace fl {

    struct WebFileMeta
    {
        std::string Name; fl::utils::MimeType Ext;
        std::filesystem::path Path;

        // returns path file name and ext
        std::string FullPath() const {
            return Path.string() + '/' + FileName();
        }
        // returns filename with extension
        std::string FileName() const {
            return Name + std::string(Ext.GetName(false));
        }
    };

    class WebFilesSystem
    {
        // web documents directory
        std::string web_root_;
        // all stored files in web site 
        // key: simplified file name only vale
        // value: full meta info about file
        std::map<std::string, WebFileMeta> files_;

    public:
        /**
         *  @param root is a directory where web documents stored
         */
        WebFilesSystem(std::string_view web_root);

        void SetWebRoot(std::string_view web_root);
        std::string_view GetWebRoot() const;
        /**
         *  @param file or name to be searched 
         *  @param extension should be included
         * 
         *  @return concat path and file name
         */
        std::string GetWebFilePath(std::string_view file, bool extension = true) const;
        /**
         *  Finds name in a searching directory
         *  @param file or name to be searched
         *  @param extension should be included
         * 
         *  @return 
         */
        std::optional<WebFileMeta> 
        GetWebFileInfo(std::string_view file, bool extension) const;
        /**
         *  Checks if file found in root
         *  @param file to be searched
         *  @param extension should be included
         * 
         *  @return 
         */
        bool IsWebFileExist(std::string_view file, bool extension = true) const;   // TODO: Implement caching 

    private: 
        /**
         *  Finds name in a searching directory
         *  @param file to be searched ONLY with extnesion 
         * 
         *  @return 
         */
        std::optional<WebFileMeta> 
        GetWebFileInfo(std::string_view file) const;

    };

}