#include "WebFilesSystem.hpp"
#include "fl/utils/Log.hpp"

namespace fl {

    std::map<std::string, WebFileMeta> 
    IterateFiles(std::filesystem::path const& directory)
    {
        std::map<std::string, WebFileMeta> files;

        for (const auto& entry : std::filesystem::directory_iterator(directory))
        {
            if (entry.is_directory())
            {
                auto const& subdirectoryFiles = IterateFiles(entry.path());
                files.insert(subdirectoryFiles.begin(), subdirectoryFiles.end());
            }
            else if (entry.is_regular_file())
            {
                std::string file = entry.path().filename().string();
                std::string name = utils::MimeType::RemoveExtension(file);
                utils::MimeType extension = utils::MimeType::FromString(file);
                            
                WebFileMeta wf = { 
                    name,
                    extension,
                    entry.path().parent_path()
                };

                files.insert(std::make_pair(name, wf));
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

    std::string WebFilesSystem::GetWebFilePath(std::string_view file, bool extension) const
    {
        auto const& web_meta = GetWebFileInfo(file, extension);
        if (!web_meta.has_value())
            return "";

        return std::string(web_meta->FullPath());
    }

    bool WebFilesSystem::IsWebFileExist(std::string_view file, bool extension) const
    {
        auto is_exist = GetWebFileInfo(file, extension).has_value();
        return is_exist;
    }

    std::optional<WebFileMeta> 
    WebFilesSystem::GetWebFileInfo(std::string_view file, bool extension) const
    {
        if (extension)
            return GetWebFileInfo(file);

        static std::optional<WebFileMeta> cached;

        if (cached.has_value() && cached->FileName() == file)
            return cached;

        for (auto const& [simplified, file_info] : files_)
        {
            if (file == simplified)
            {
                cached.emplace(file_info);
                return file_info;
            }
        }

        return std::nullopt;
    }
    std::optional<WebFileMeta> 
    WebFilesSystem::GetWebFileInfo(std::string_view file) const 
    {
        static std::optional<WebFileMeta> cached;

        if (cached.has_value() && cached->FileName() == file)
            return cached;

        for (auto const& [simplified, file_info] : files_)
        {
            std::string full_name = file_info.FileName();

            if (file == full_name)
            {
                cached.emplace(file_info);
                return file_info;
            }
        }

        return std::nullopt;
    }
}