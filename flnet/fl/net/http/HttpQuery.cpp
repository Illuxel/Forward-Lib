#include "fl/net/http/HttpQuery.hpp"
#include "fl/utils/StringArg.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <codecvt>

#include <algorithm>
#include <iterator>

namespace fl {

    std::string UrlEncodeUtf8(std::string_view input) {
        std::ostringstream encoded;
        encoded << std::hex << std::uppercase << std::setfill('0');

        for (const unsigned char c : input) {
            if (c <= 0x7F) {
                if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
                    encoded << c;
                } else {
                    encoded << '%' << std::setw(2) << static_cast<int>(c);
                }
            } else {
                encoded << '%' << std::setw(2) << static_cast<int>(c);
            }
        }
        return encoded.str();
    }
    std::string UrlDecodeUtf8(std::string_view input) {
        std::ostringstream decoded;
        for (std::size_t i = 0; i < input.size(); ++i) {
            if (input[i] == '%') {
                auto hexCode = input.substr(i + 1, 2);
                unsigned char decodedChar = std::stoi(hexCode.data(), nullptr, 16);
                decoded << decodedChar;
                i += 2; // Skip the two hexadecimal characters
            } else if (input[i] == '+') {
                decoded << ' '; // Replace '+' with space
            } else {
                decoded << input[i]; // Append other characters as is
            }
        }
        return decoded.str();
    }

    HttpQuery::HttpQuery() 
        : is_valid(false){}
    HttpQuery::HttpQuery(std::string_view query)
        : is_valid(false)
    {
        auto decoded = UrlDecodeUtf8(query);
        SetQuery(decoded);
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

            auto const& param = view.substr(0, 
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

        if (params_.has_value())
            is_valid = true;
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

        if (!IsValid())
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

        if (!IsValid())
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

        if (!IsValid())
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

    size_t HttpQuery::Size() const
    {
        if (!params_.has_value())
            return 0;

        return params_->size();
    }

    bool HttpQuery::IsValid() const 
    {
        return is_valid;
    }

    bool HttpQuery::HasKey(std::string_view key) const 
    {
        if (!IsValid())
            return false;

        return params_->find(key.data()) != params_->cend();
    }
}