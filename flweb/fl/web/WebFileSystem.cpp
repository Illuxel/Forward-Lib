#include "fl/web/WebFileSystem.hpp"
#include "fl/core/Log.hpp"

#include <algorithm>

namespace Forward::Web {

    std::vector<WebFileInfo> 
    IterateFiles(std::filesystem::path const& dir, std::filesystem::path base = "")
    {
        std::vector<WebFileInfo> files;

        if (base.empty())
            base = dir;

        for (auto const& entry : std::filesystem::directory_iterator(dir))
        {
            if (entry.is_directory())
            {
                auto const& subdirectoryFiles = IterateFiles(entry.path(), base);
                files.insert(files.end(), subdirectoryFiles.begin(), subdirectoryFiles.end());
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

                WebFileInfo wf;

                wf.Name = file_name;
                wf.Ext = MimeType::FromString(file);
                wf.Relative = relative_target;
                wf.Base = base;

                files.push_back(wf);
            }
        }

        return files;
    }

    WebFilesSystem::WebFilesSystem() {}
    WebFilesSystem::WebFilesSystem(std::string_view web_root) 
    {
        if (!std::filesystem::is_directory(web_root)) 
            return;

        SetWebRoot(web_root);
    }

    void WebFilesSystem::SetWebRoot(std::string_view web_root) 
    {
        std::unique_lock lock(wfs_mutex_);
        web_root_ = web_root;
        files_ = IterateFiles(web_root_);

        is_valid = !files_.empty();
    }
    std::string_view WebFilesSystem::GetWebRoot() const 
    {
        std::shared_lock lock(wfs_mutex_);
        return web_root_;
    }

    std::optional<WebFileInfo> 
    WebFilesSystem::FindByTargetName(std::string_view target, bool extension) const
    {
        static std::optional<WebFileInfo> cached = std::nullopt;

        if (cached.has_value() && cached->TargetName(extension) == target)
            return cached;

        auto const& it = std::find_if(files_.cbegin(), files_.cend(), 
        [&target, &extension](auto const& file_info) 
        {
            auto target_name = file_info.TargetName(extension);
            return target == target_name;
        });

        if (it != files_.cend())
        {
            {    
                std::shared_lock lock(wfs_mutex_);
                cached.emplace(*it);
            }

            return *it;
        }

        return std::nullopt;
    }

    std::optional<WebFileInfo> 
    WebFilesSystem::FindByTargetPath(std::string_view target, bool extension) const
    {
        static std::optional<WebFileInfo> cached = std::nullopt;

        if (cached.has_value() && cached->TargetPath(extension) == target)
            return cached;

        auto const& it = std::find_if(files_.cbegin(), files_.cend(), 
        [&target, &extension](auto const& file_info)
        {
            auto target_path = file_info.TargetPath(extension);
            return target == target_path;
        });

        if (it != files_.cend()) 
        {
            {    
                std::shared_lock lock(wfs_mutex_);
                cached.emplace(*it);
            }

            return *it;
        }

        return std::nullopt;
    }

    bool WebFilesSystem::IsValid() const
    {
        std::shared_lock lock(wfs_mutex_);
        return is_valid;
    }

    bool WebFilesSystem::IsTargetNameExist(std::string_view target, bool extension) const
    {
        std::shared_lock lock(wfs_mutex_);
        return FindByTargetName(target, extension).has_value();
    }
    bool WebFilesSystem::IsTargetPathExist(std::string_view target, bool extension) const
    {
        std::shared_lock lock(wfs_mutex_);
        return FindByTargetPath(target, extension).has_value();
    }
}