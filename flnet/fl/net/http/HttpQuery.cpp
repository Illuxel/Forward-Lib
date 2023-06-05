#include "fl/net/http/HttpQuery.hpp"
#include "fl/utils/StringArg.hpp"

#include <algorithm>
#include <iterator>

using namespace fl::utils;

namespace fl {

    HttpQuery::HttpQuery() {}
    HttpQuery::HttpQuery(std::string_view query)
    {
        SetQuery(query);
    }

    void HttpQuery::SetQuery(std::string_view query)
    {
        if (query.empty()) 
            return;

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

            if (!params_.has_value())
                params_.emplace();

            params_->insert(std::make_pair(arg.Name(), arg.Data()));

            view = view.substr(
                is_sep
                ? param.size() + 1
                : param.size(), view.size());
        }
    }

    StringArg HttpQuery::Arg(std::string_view key) const
    {
        StringArg arg;

        if (!HasKey(key))
            return arg;

        arg.SetName(key);
        arg.SetData(Value(key));

        return arg;
    }

    std::string_view HttpQuery::Value(std::string_view key) const 
    {
        if (!HasKey(key)) 
            return "";

        return params_->at(key.data());
    }

    std::vector<std::string> HttpQuery::Keys() const
    {
        std::vector<std::string> keys;

        if (IsEmpty())
            return keys;

        keys.reserve(params_->size());

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
        std::string temp;

        if (IsEmpty())
            return temp;

        for (auto const& [key, val] : params_.value())
        {
            if (!temp.empty())
                temp.push_back('&');

            temp.append(key + '=' + val);
        }

        return temp;
    }
    
    std::vector<StringArg> HttpQuery::ToArgs() const 
    {
        std::vector<StringArg> args;

        if (IsEmpty())
            return args;

        args.reserve(params_->size());

        std::transform(
            params_->cbegin(), 
            params_->cend(), 
            std::back_inserter(args), 
            [](auto const& pair){ return StringArg(pair.first, pair.second); }
        );

        return args;
    }
    std::vector<StringArg> HttpQuery::ToArgs(char specifier) const 
    {
        auto& args = ToArgs();

        for (auto& arg : args)
            arg.SetSpecifier(specifier);

        return args;
    }

    bool HttpQuery::IsEmpty() const
    {
        if (!params_.has_value())
            return true;

        return params_->empty();
    }

    bool HttpQuery::HasKey(std::string_view key) const 
    {
        if (IsEmpty())
            return false;

        return params_->find(key.data()) != params_->cend();
    }
}