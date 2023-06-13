#pragma once

#include "fl/utils/MimeType.hpp"

#include <vector>

#include <optional>
#include <filesystem>

namespace fl {

    struct WebFileMeta
    {
        std::string Name; MimeType Ext;
        std::string Relative;
        std::filesystem::path Base;

        std::string File(bool extension = true) const {
            return Name + (extension
                             ? Ext.GetExtName(false)
                             : "");
        }
        // returns path file name and ext
        std::string FullPath() const {
            return Base.string() + TargetPath();
        }

        // return target file name
        std::string TargetName(bool extension = true) const {
            return '/' + File(extension);
        }
        // return relative path and file
        std::string TargetPath(bool extension = true) const {
            if (Relative == Base.filename())
                return TargetName(extension);

            return '/' + Relative + TargetName(extension);
        }

        bool operator==(WebFileMeta const& right) const 
        {
            return this->TargetPath() == right.TargetPath();
        }
    };

    class WebFilesSystem
    {
        // web documents directory
        std::string web_root_;
        // all stored files in web site 
        // key: simplified file name only vale
        // value: full meta info about file
        std::vector<WebFileMeta> files_;

    public:
        /**
         *  @param root is a directory where web documents stored
         */
        WebFilesSystem(std::string_view web_root);

        void SetWebRoot(std::string_view web_root);
        std::string_view GetWebRoot() const;

        // std::list<WebFileMeta> FindByLastFolder(std::string_view folder);
        // search file by name
        std::optional<WebFileMeta> FindByTargetName(std::string_view target, bool extension) const;
        // 
        std::optional<WebFileMeta> FindByTargetPath(std::string_view target, bool extension = true) const;

        bool IsTargetNameExist(std::string_view target, bool extension) const;  
        bool IsTargetPathExist(std::string_view target, bool extension) const;
    };

}