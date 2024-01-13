#include "fl/web/HttpQuery.hpp"

#include "fl/core/StringArg.hpp"
#include "fl/core/StringArgParser.hpp"

#include "fl/web/HttpUrlParser.hpp"

#include <sstream>
#include <iomanip>

#include <algorithm>
#include <iterator>

namespace Forward::Web {

    HttpQuery::HttpQuery() {}
    HttpQuery::HttpQuery(std::string_view query)
    {
        if (query.empty())
            return;

        std::string decoded = HttpUrlParser::DecodeUtf8(query);

        SetQuery(decoded);
    }

    void HttpQuery::SetQuery(std::string_view query)
    {
        if (query.empty()) 
            return;

        std::string_view view = query;

        if (query.front() == '?')
            view = query.substr(0, 1);

        while (true)
        {
            if (view.empty())
                break;

            uint64_t sep_pos = view.find('&');
            uint64_t delim_pos = view.find('=');

            bool is_sep = sep_pos != std::string::npos;
            bool is_delim = delim_pos != std::string::npos;

            if (!is_delim) break;

            std::string_view param = view.substr(0,
                is_sep
                ? sep_pos
                : view.size()
            );

            StringArg arg = StringArgParser::FromString(param);

            args_.insert(std::make_pair(arg.GetName(), arg.GetData()));

            view = view.substr(
                is_sep
                ? param.size() + 1
                : param.size(), view.size());
        }

        is_valid = true;
    }

    StringArg HttpQuery::Arg(std::string_view key) const
    {
        if (!HasKey(key))
            return StringArg();

        return StringArg(key, Value(key).data());
    }

    std::string_view HttpQuery::Value(std::string_view key) const 
    {
        if (!HasKey(key)) 
            return "";

        return args_.at(key.data());
    }

    std::vector<std::string> HttpQuery::Keys() const
    {
        std::vector<std::string> keys;

        if (!IsValid())
            return keys;

        keys.reserve(Size());

        std::transform(
            args_.cbegin(),
            args_.cend(),
            std::back_inserter(keys),
            [](auto const& pair)
            { 
                return pair.first; 
            });

        return keys;
    }

    std::string HttpQuery::ToString() const
    {
        std::string temp = "?";

        if (!IsValid())
            return temp;

        for (auto const& [key, val] : args_)
        {
            if (!temp.empty())
                temp.push_back('&');

            temp.append(key + '=' + val);
        }

        return temp;
    }
    
    StringArgList HttpQuery::ToArgs(std::string_view format) const
    {
        StringArgList args;

        if (!IsValid())
            return args;

        args.reserve(Size());

        std::transform(
            args_.cbegin(),
            args_.cend(),
            std::back_inserter(args), 
            [&format](auto const& pair) 
            {
                return StringArg(pair.first, pair.second, format); 
            });

        return args;
    }

    uint64_t HttpQuery::Size() const
    {
        return args_.size();
    }

    bool HttpQuery::IsValid() const 
    {
        return is_valid;
    }

    bool HttpQuery::HasKey(std::string_view key) const 
    {
        if (!IsValid())
            return false;

        return args_.find(key.data()) != args_.cend();
    }

    std::string& HttpQuery::operator[](std::string_view key)& 
    {
        return args_[key.data()];
    }
}