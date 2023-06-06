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

    std::optional<WebFileMeta> 
    WebFilesSystem::FindWebFile(std::string_view file_name, bool extension) const
    {
        if (extension)
            return FindWebFile(file_name);

        static std::optional<WebFileMeta> cached;

        if (cached.has_value() && cached->File() == file_name)
            return cached;

        auto const& it = std::find_if(files_.begin(), files_.end(), 
        [&file_name](auto const& pair){
            
            std::string_view simplified = pair.first;
            if (file_name != simplified)
                return false;

            cached.emplace(pair.second);
            return true;
        });

        if (it == files_.end())
            return std::nullopt;

        return it->second;
    }
    std::optional<WebFileMeta> 
    WebFilesSystem::FindWebFile(std::string_view file_name) const 
    {
        static std::optional<WebFileMeta> cached;

        if (cached.has_value() && cached->File() == file_name)
            return cached;

        auto const& it = std::find_if(files_.begin(), files_.end(), 
        [&file_name](auto const& pair){

            std::string_view file = pair.second.File();
            if (file_name != file)
                return false;

            cached.emplace(pair.second);
            return true;
        });

        if (it == files_.end())
            return std::nullopt;

        return it->second;
    }

    bool WebFilesSystem::IsWebFileExist(std::string_view file, bool extension) const
    {
        return FindWebFile(file, extension).has_value();
    }
}