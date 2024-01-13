#include "fl/web/HttpRouter.hpp"
#include "fl/core/Log.hpp"

namespace Forward::Web {

    HttpRouter::HttpRouter()
    {
        
    }

    HttpRouter::HttpRouter(std::string_view web_root
        , std::string_view index
        , std::string_view ext)
    {
        def_ext_ = MimeType::FromString(ext);

        def_route_.append(index);
        def_route_.append(def_ext_.GetExtName(false));
        
        if (def_route_.front() != '/')
            def_route_.insert(def_route_.begin(), '/');

        wfs_ = MakeScope<WebFilesSystem>(web_root);
    }

    std::string HttpRouter::GetPreparedTarget(std::string_view name) const 
    {
        std::shared_lock lock(mtx_);

        auto const& it = routes_.find(name.data());
        
        if (it == routes_.cend())
            return "";

        return it->second;
    }

    std::string HttpRouter::GetRouteFilePath(std::string_view target) const
    {
        auto const& prep_route = GetPreparedTarget(target);
        auto const& web_file = wfs_->FindByTargetName(prep_route, MimeType::HasExtension(prep_route));

        if (!web_file.has_value())
            return "";
        return web_file->FullPath();
    }
    std::string HttpRouter::GetContentFilePath(std::string_view content) const
    {
        auto const& web_file = wfs_->FindByTargetPath(content);

        if (!web_file.has_value())
            return "";
        return web_file->FullPath();
    }

    void HttpRouter::RegisterTarget(std::string_view target)
    {
        auto const& prep_route = PrepareRouteName(target);

        std::unique_lock lock(mtx_);
        routes_.insert(std::make_pair(target.data(), prep_route));
    }
    void HttpRouter::RegisterContent(std::string_view target)
    {
        std::string temp(target.data(), target.size());

        std::unique_lock lock(mtx_);

        if (target.front() == '/')
            content_.insert(temp);
        else 
            content_.insert("/" + temp);
    }

    bool HttpRouter::IsTarget(std::string_view target) const
    {
        std::shared_lock lock(mtx_);
        return routes_.find(target.data()) != routes_.end();
    }
    bool HttpRouter::IsContent(std::string_view target) const
    {
        if (content_.empty() && wfs_->IsTargetPathExist(target, true))
            return true;

        if (content_.find(target.data()) != content_.cend() 
            && wfs_->IsTargetPathExist(target, true))
            return true;

        return false;
    }

    bool HttpRouter::IsTargetIndex(std::string_view target) const
    {   
        if (target.empty())
            return false;
        if (target.back() == '/')
            return true;

        std::shared_lock lock(mtx_);

        if (target == def_route_)
            return true;
        if (target == MimeType::RemoveExtension(def_route_))
            return true;

        return false;
    }

    std::string HttpRouter::PrepareRouteName(std::string_view target) const
    {
        std::shared_lock lock(mtx_);

        if (IsTargetIndex(target))
            return def_route_;

        if (!MimeType::HasExtension(target))
            return std::string(target.data() + def_ext_.GetExtName(false));

        return target.data();
    }
}