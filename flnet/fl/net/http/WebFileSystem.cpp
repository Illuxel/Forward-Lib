#include "fl/net/http/WebFileSystem.hpp"
#include "fl/utils/Log.hpp"

#include <algorithm>

namespace fl {

    std::unordered_set<WebFileMeta, WebFileMetaHash> 
    IterateFiles(std::filesystem::path const& dir, std::filesystem::path base = "")
    {
        std::unordered_set<WebFileMeta, WebFileMetaHash> files;

        if (base.empty())
            base = dir;

        for (auto const& entry : std::filesystem::directory_iterator(dir))
        {
            if (entry.is_directory())
            {
                auto const& subdirectoryFiles = IterateFiles(entry.path(), base);
                files.insert(subdirectoryFiles.begin(), subdirectoryFiles.end());
            }
            else if (entry.is_regular_file())
            {
                std::string relative_target,
                            file = entry.path().filename().string(),
                            file_name = MimeType::RemoveExtension(file);
                
                if (dir == base)
                {
                    relative_target = entry.path().parent_path().filename().string();
                }
                else
                {
                    auto relative = std::filesystem::relative(entry.path(), base);
                    relative.remove_filename();
                    auto rel_gen = relative.generic_string();
                    
                    relative_target = rel_gen.substr(0, rel_gen.size() - 1);
                }

                WebFileMeta wf = { 
                    file_name, 
                    MimeType::FromString(file),
                    relative_target,
                    base
                };

                files.insert(wf);
            }
        }

        return files;
    }

    WebFilesSystem::WebFilesSystem(std::string_view web_root) 
    {
        SetWebRoot(web_root);
    }

    void WebFilesSystem::SetWebRoot(std::string_view web_root) 
    {
        if (!std::filesystem::is_directory(web_root)) 
        {
            FL_LOG("WebFilesSystem", "Unknown directory");
            return;
        }

        web_root_ = web_root;
        files_ = IterateFiles(web_root_);
    }
    std::string_view WebFilesSystem::GetWebRoot() const 
    {
        return web_root_;
    }

    std::optional<WebFileMeta> 
    WebFilesSystem::FindByTargetName(std::string_view target, bool extension) const
    {
        static std::optional<WebFileMeta> cached;

        if (cached.has_value()
         && cached->TargetName(extension) == target)
            return cached;

        auto const& it = std::find_if(files_.begin(), files_.end(), 
        [&target, &extension](auto const& file_info) 
        {
            auto target_name = file_info.TargetName(extension);
            if (target != target_name)
                return false;

            cached.emplace(file_info);
            return true;
        });

        if (it != files_.end())
            return *it;

        return std::nullopt;
    }

    std::optional<WebFileMeta> 
    WebFilesSystem::FindByTargetPath(std::string_view target, bool extension) const
    {
        static std::optional<WebFileMeta> cached;

        if (cached.has_value()
         && cached->TargetPath(extension) == target)
            return cached;

        auto const& it = std::find_if(files_.begin(), files_.end(), 
        [&target, &extension](auto const& file_info)
        {
            auto target_path = file_info.TargetPath(extension);
            if (target != target_path)
                return false;

            cached.emplace(file_info);
            return true;
        });

        if (it != files_.end())
            return *it;

        return std::nullopt;
    }

    bool WebFilesSystem::IsTargetNameExist(std::string_view target, bool extension) const
    {
        return FindByTargetName(target, extension).has_value();
    }
    bool WebFilesSystem::IsTargetPathExist(std::string_view target, bool extension) const
    {
        return FindByTargetPath(target, extension).has_value();
    }
}