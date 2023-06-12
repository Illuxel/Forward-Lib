#include "fl/net/http/HttpRouter.hpp"
#include "fl/utils/Log.hpp"

using namespace fl;

namespace fl {

    HttpRouter::HttpRouter(std::string_view web_root
        , std::string_view index
        , std::string_view ext)
    {
        index_.push_back('/');
        index_.append(index);
        def_ext_ = MimeType::FromString(ext);
        index_.append(def_ext_.GetExtName(false));
        wfs_ = MakeScope<WebFilesSystem>(web_root);
    }

    void HttpRouter::AddConentFolder(std::string_view folder_name)
    {
        content_folder_.push_back(folder_name.data());
    }

    void HttpRouter::Register(std::string_view target) 
    {
        this->Register(target, std::nullopt);
    }
    void HttpRouter::Register(std::string_view target, std::optional<HttpRouter::Callback> const& handler) 
    {
        auto valid_route = MakeValidRoute(target);

        if (routes_.find(valid_route) != routes_.end())
        {
            FL_LOG("HttpRouter", "Target already registered");
            return;
        }

        routes_.insert(std::make_pair(valid_route, handler));
    }
    
    std::optional<HttpRouter::Callback> HttpRouter::FindRouteCallback(std::string_view target) const
    {
        auto valid_route = MakeValidRoute(target);
        auto const& it = routes_.find(valid_route);

        if (it != routes_.end())
            return it->second;

        return std::nullopt;
    }

    std::string HttpRouter::GetTargetPath(std::string_view target) const
    {
        auto valid_route = MakeValidRoute(target);
        auto const& web_file = wfs_->FindWebFileByTarget(valid_route, MimeType::HasExtension(valid_route));

        if (!web_file.has_value())
            return "";

        return web_file->FullPath();
    }
    std::string HttpRouter::GetContentPath(std::string_view content) const
    {
        auto const& web_file = wfs_->FindWebFileByTargetPath(content);

        if (!web_file.has_value())
            return "";

        return web_file->FullPath();
    }

    bool HttpRouter::IsTarget(std::string_view target) const
    {
        auto valid_route = MakeValidRoute(target);
        return routes_.find(valid_route.data()) != routes_.end();
    }
    bool HttpRouter::IsTargetIndex(std::string_view target) const
    {   
        if (target.empty())
            return false;

        if (target.back() == '/')
            return true;

        // check default index name
        if (target == index_)
            return true;
        if (target == MimeType::RemoveExtension(index_))
            return true;

        return false;
    }

    std::string HttpRouter::MakeValidRoute(std::string_view target) const
    {
        if (!IsTargetValid(target))
            return index_;

        if (IsTargetIndex(target))
            return index_;

        if (!MimeType::HasExtension(target))
            return std::string(target.data() + def_ext_.GetExtName(false));

        return target.data();
    }

    bool HttpRouter::IsTargetValid(std::string_view target)
    {
        if (target.empty() ||
            target.front() != '/' ||
            target.find("..") != std::string_view::npos)
            return false;
        
        return true;
    }

    bool HttpRouter::IsContent(std::string_view target) const
    {
        return wfs_->IsWebFileTargetPathExist(target, true);
    }
}