#pragma once

#include "fl/utils/MimeType.hpp"

#include "fl/net/Core.hpp"

#include <map>
#include <optional>
#include <filesystem>

namespace fl {

    struct WebFileMeta
    {
        std::string Name, LastFolder; 
        MimeType Ext;
        std::filesystem::path Path;

        // returns filename with extension
        std::string File() const {
            return Name + std::string(Ext.GetExtName(false));
        }
        // returns path file name and ext
        std::string FullPath() const {
            return Path.string() + '/' + File();
        }

        // return target file name with extension or without
        std::string TargetName(bool extension = true) const {
            return '/' + Name + (extension
                      ? std::string(Ext.GetExtName(false))
                      : "");
        }
        // return target file folder and file name
        std::string TargetPath() const {
            return '/' + LastFolder + '/' + File();
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

        // search file by name
        std::optional<WebFileMeta> FindWebFile(std::string_view file, bool extension) const;

        bool IsWebFileExist(std::string_view file, bool extension = true) const;   // TODO: Implement caching 

    private: 
        std::optional<WebFileMeta> 
        FindWebFile(std::string_view file) const;

    };

}