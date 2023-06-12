#include "WebFilesSystem.hpp"
#include "fl/utils/Log.hpp"

#include <algorithm>


namespace fl {

    std::list<WebFileMeta> 
    IterateFiles(std::filesystem::path const& directory)
    {
        std::list<WebFileMeta> files;

        for (auto const& entry : std::filesystem::directory_iterator(directory))
        {
            if (entry.is_directory())
            {
                auto const& subdirectoryFiles = IterateFiles(entry.path());
                files.insert(files.end(), subdirectoryFiles.begin(), subdirectoryFiles.end());
            }
            else if (entry.is_regular_file())
            {
                std::string last_folder = entry.path()
                                               .parent_path()
                                               .filename().string();
                std::string file = entry.path()
                                        .filename().string();

                std::string name = MimeType::RemoveExtension(file);
                MimeType extension = MimeType::FromString(file);
                            
                WebFileMeta wf = { 
                    name, 
                    last_folder,
                    extension,
                    entry.path().parent_path()
                };

                files.push_back(wf);
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
    WebFilesSystem::FindWebFileByTarget(std::string_view target, bool extension) const
    {
        static std::optional<WebFileMeta> cached;

        if (cached.has_value()
         && cached->TargetName(extension) == target)
            return cached;

        auto const& it = std::find_if(files_.begin(), files_.end(), 
        [&target, &extension](auto const& file_info){
            
            auto file = file_info.TargetName(extension);
            if (target != file)
                return false;

            cached.emplace(file_info);
            return true;
        });

        if (it != files_.end())
            return *it;

        return std::nullopt;
    }

    std::optional<WebFileMeta> 
    WebFilesSystem::FindWebFileByTargetPath(std::string_view target) const
    {
        static std::optional<WebFileMeta> cached;

        bool extension = MimeType::HasExtension(target);

        if (cached.has_value()
         && cached->TargetPath(extension) == target)
            return cached;

        auto const& it = std::find_if(files_.begin(), files_.end(), 
        [&target, &extension](auto const& file_info)
        {
            if (target != file_info.TargetPath(extension))
                return false;

            cached.emplace(file_info);

            return true;
        });

        if (it != files_.end())
            return *it;

        return std::nullopt;
    }

    bool WebFilesSystem::IsWebFileTargetExist(std::string_view target, bool extension) const
    {
        return FindWebFileByTarget(target, extension).has_value();
    }
    bool WebFilesSystem::IsWebFileTargetPathExist(std::string_view target, bool extension) const
    {
        return FindWebFileByTargetPath(target).has_value();
    }
}