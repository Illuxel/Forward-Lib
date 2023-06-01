#include "fl/net/http/HttpRouter.hpp"

#include "fl/utils/Log.hpp"

namespace fl {

    HttpRouter::HttpRouter(std::string_view web_root, std::string_view index)
        : index_(index)
    {
        wfs_ = MakeRef<WebFilesSystem>(web_root);
    }

    void HttpRouter::SetMainPage(std::string_view index)
    {
        index_ = index;
    }
    std::string_view HttpRouter::GetMainPage() const
    {
        return index_;
    }

    std::string HttpRouter::GetTargetPath(std::string_view target) const
    {
        auto const& valid_route = PrepareRoute(target);
        auto const& web_file = wfs_->GetWebFileInfo(valid_route, utils::MimeType::HasExtension(valid_route));

        if (!web_file.has_value())
            return "";

        return web_file->FullPath();
    }

    std::string HttpRouter::GetContentPath(std::string_view content) const
    {
        std::string_view only_name = content.substr(1, content.size());
        auto const& web_file = wfs_->GetWebFileInfo(only_name, utils::MimeType::HasExtension(only_name));

        if (!web_file.has_value())
            return "";

        return web_file->FullPath();
    }

    bool HttpRouter::IsTargetIndex(std::string_view target) const
    {   
        if (target.empty())
            return false;

        // auto index_name = MimeType::RemoveExtension(index_); 
        // auto target_name = target.substr(1, target.size());

        if (target.back() == '/')
            return true;

        return false;
    }
    bool HttpRouter::IsTargetRegisted(std::string_view target) const
    {
        bool is_registed = routes_.find(target.data()) != routes_.end();
        return is_registed;
    }
    bool HttpRouter::IsTargetLegal(std::string_view target)
    {
        if (target.empty() ||
            target.front() != '/' ||
            target.find("..") != std::string_view::npos)
            return false;

        return true;
    }

    bool HttpRouter::IsContentTarget(std::string_view target) const
    {
        std::string_view only_name = target.substr(1, target.size());

        bool is_exist = wfs_->IsWebFileExist(only_name);
        bool is_content = !IsTargetRegisted(only_name) && is_exist;

        return is_content;
    }

    std::string HttpRouter::PrepareRoute(std::string_view target) const
    {
        if (IsTargetIndex(target))
            return index_;

        if (!IsTargetLegal(target))
        {
            FL_LOG("HttpRouter", "illegal route");
            return index_;
        }

        std::string only_name(target.substr(1, target.size()));

        if (!wfs_->IsWebFileExist(only_name,  utils::MimeType::HasExtension(only_name)))
            return index_;

        return only_name;
    }

    void HttpRouter::Register(std::string_view target, HttpRouter::Callback const& handler) 
    {
        if (routes_.find(target.data()) != routes_.end())
        {
            FL_LOG("HttpRouter", "Target already registered");
            return;
        }

        routes_.insert(std::make_pair(target.data(), handler));
    }
    std::optional<HttpRouter::Callback> HttpRouter::FindRouteCallback(std::string_view target) const
    {
        auto const& it = routes_.find(target.data());

        if (it != routes_.end())
            return it->second;

        return std::nullopt;
    }

}