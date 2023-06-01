#include "fl/net/http/HttpUrl.hpp"

#include "fl/utils/StringArg.hpp"

#include <algorithm>
#include <iterator>

namespace fl {
    
    HttpUrl::HttpUrl() {}
    HttpUrl::HttpUrl(std::string_view url)
    {
        using namespace fl::utils;

        auto start_domain = url.find("//");
        bool is_domain = start_domain != std::string::npos;
        auto start_target = url.find('/', is_domain ? start_domain + 2 : 0); 

        if (is_domain)
            domain_.emplace(
                url.cbegin() + start_domain + 2, // to skip slashes
                url.cbegin() + start_target
            );

        auto end_target = url.find('?', start_target);

        bool is_target = start_target != std::string::npos
                      && end_target != std::string::npos;

        if (!is_target) return;

        target_.assign(
            url.begin() + start_target, 
            url.begin() + end_target
        );

        auto start_section = url.find('#');
        bool is_section = start_section != std::string::npos;

        if (is_section)
            section_.emplace(
                url.cbegin() + start_section + 1, // + 1 to skip #
                url.cbegin() + url.size()
            ); 

        std::string str_params(
            url.cbegin() + end_target + 1, // + 1 to skip ?
            is_section
            ? url.cend() - section_->size() - 1 // - 1 to skip #
            : url.cend()
        );
        
        while (true)
        {
            if (str_params.empty()) break;

            auto const sep_pos = str_params.find('&');
            auto const delim_pos = str_params.find('=');

            bool const is_sep = sep_pos != std::string::npos;
            bool const is_delim = delim_pos != std::string::npos;

            if (!is_delim) break;

            std::string param(str_params.cbegin(), 
                is_sep
                ? str_params.cbegin() + sep_pos
                : str_params.cend()
            );
            auto const& arg = StringArg::FromString(param);

            if (!query_params_) query_params_.emplace();

            query_params_->insert(std::make_pair(arg.Name(), arg.Data()));
            str_params.erase(str_params.cbegin(), 
                is_sep 
                ? str_params.cbegin() + sep_pos + 1 // + 1 to skip &
                : str_params.cend());
        }
    }

    std::string_view HttpUrl::Target() const
    {
        return target_;
    }
    std::string_view HttpUrl::Domain() const
    {
        if (!domain_) return "";
        return domain_.value();
    }
    std::string_view HttpUrl::Section() const
    {
        if (!section_) return "";
        return section_.value();
    }
    std::string_view HttpUrl::Value(std::string_view key) const 
    {
        if (!HasKey(key)) return "";

        return query_params_->at(key.data());
    }

    std::list<std::string> HttpUrl::Keys() const
    {
        std::list<std::string> keys;

        std::transform(
            query_params_->begin(), 
            query_params_->end(), 
            std::back_inserter(keys), 
            [](auto const& pair){ return pair.first; }
        );

        return keys;
    }
    
    bool HttpUrl::HasDomain() const
    {
        return domain_.has_value();
    }
    bool HttpUrl::HasTarget() const 
    {
        return !target_.empty();
    }
    bool HttpUrl::HasSection() const
    {
        return section_.has_value();
    }
    bool HttpUrl::HasParameters() const
    {
        return query_params_.has_value();
    }
    bool HttpUrl::HasKey(std::string_view key) const 
    {
        if (!HasParameters()) 
            return false;

        return query_params_->find(key.data()) != query_params_->cend();
    }

    bool HttpUrl::IsValueEmpty(std::string_view key) const
    {
        if (!HasParameters()) 
            return false;

        auto const& it = query_params_->find(key.data());
        auto const has_value = it != query_params_->cend()
                            || it->second.empty();

        return has_value;
    }

} // namespace fl
