#include "fl/net/http/HttpRouter.hpp"
#include "fl/utils/Log.hpp"

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

    std::string HttpRouter::GetRoutePath(std::string_view target) const
    {
        auto const& valid_route = MakeRouteFromName(target);
        auto const& web_file = wfs_->FindByTargetName(valid_route, MimeType::HasExtension(valid_route));

        if (!web_file.has_value())
            return "";

        return web_file->FullPath();
    }
    std::string HttpRouter::GetContentPath(std::string_view content) const
    {
        auto const& web_file = wfs_->FindByTargetPath(content);

        if (!web_file.has_value())
            return "";

        return web_file->FullPath();
    }

    void HttpRouter::RegisterRoute(std::string_view target) 
    {
        auto const& valid_route = MakeRouteFromName(target);
        routes_.insert(valid_route);
    }
    void HttpRouter::AddConentFolder(std::string_view folder_name)
    {
        content_folder_.insert(folder_name.data());
    }

    std::string HttpRouter::MakeRouteFromName(std::string_view target) const
    {
        if (!IsTargetValid(target))
            return index_;

        if (IsTargetIndex(target))
            return index_;

        if (!MimeType::HasExtension(target))
            return std::string(target.data() + def_ext_.GetExtName(false));

        return target.data();
    }

    bool HttpRouter::IsTarget(std::string_view target) const
    {
        auto const& valid_route = MakeRouteFromName(target);
        return routes_.find(valid_route.data()) != routes_.end();
    }
    bool HttpRouter::IsContent(std::string_view target) const
    {
        return wfs_->IsTargetPathExist(target, true);
    }

    bool HttpRouter::IsTargetValid(std::string_view target)
    {
        if (target.empty() ||
            target.front() != '/' ||
            target.find("..") != std::string_view::npos)
            return false;
        
        return true;
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
}