#include "fl/net/http/HttpQuery.hpp"
#include "fl/utils/StringArg.hpp"

#include <algorithm>
#include <iterator>

namespace fl {

    HttpQuery::HttpQuery(std::string_view query)
    {
        using namespace fl::utils;

        if (query.empty()) return;

        params_.emplace();

        std::string_view view = query;

        while (true)
        {
            if (view.empty()) break;

            auto const sep_pos = view.find('&');
            auto const delim_pos = view.find('=');

            bool const is_sep = sep_pos != std::string::npos;
            bool const is_delim = delim_pos != std::string::npos;

            if (!is_delim) break;

            std::string_view param = view.substr(0, 
                is_sep
                ? sep_pos
                : view.size()
            );

            auto const& arg = StringArg::FromString(param);
            params_->insert(std::make_pair(arg.Name(), arg.Data()));

            view = view.substr(
                is_sep
                 ? param.size() + 1
                 : param.size(), 
                is_sep 
                 ? sep_pos + 1 // + 1 to skip &
                 : param.size());
        }
    }

    std::string_view HttpQuery::Value(std::string_view key) const 
    {
        if (!HasKey(key)) return "";
        return params_->at(key.data());
    }

    std::list<std::string> HttpQuery::Keys() const
    {
        std::list<std::string> keys;

        if (!HasKeys())
            return keys;

        std::transform(
            params_->cbegin(), 
            params_->cend(), 
            std::back_inserter(keys), 
            [](auto const& pair){ return pair.first; }
        );

        return keys;
    }

    std::string HttpQuery::ToString() const
    {
        if (!HasKeys())
            return "";

        std::string temp;

        for (auto const& [key, val] : params_.value())
        {
            if (!temp.empty())
                temp.push_back('&');

            temp.append(key + '=' + val);
        }

        return temp;
    }

    bool HttpQuery::HasKeys() const
    {
        return params_.has_value();
    }
    bool HttpQuery::HasKey(std::string_view key) const 
    {
        if (!HasKeys()) 
            return false;

        return params_->find(key.data()) != params_->cend();
    }

    bool HttpQuery::IsValueEmpty(std::string_view key) const
    {
        if (!HasKeys()) 
            return false;

        auto const& it = params_->find(key.data());
        auto const has_value = it != params_->cend()
                            || it->second.empty();

        return has_value;
    }
}