#include "fl/net/http/HttpQuery.hpp"

#include "fl/utils/StringArg.hpp"
#include "fl/utils/StringArgParser.hpp"

#include <sstream>
#include <iomanip>
#include <codecvt>

#include <algorithm>
#include <iterator>

namespace Forward::Web {

    std::string UrlEncodeUtf8(std::string_view input)
    {
        std::ostringstream encoded;

        encoded << std::hex << std::uppercase << std::setfill('0');

        for (unsigned char c : input)
        {
            if (c > 127)
            {
                encoded << '%' << std::setw(2) << static_cast<unsigned int>(c);
                continue;
            }

            if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') 
            {
                encoded << c;
            }
            //else if (c == ' ') 
            //{
            //    encoded << '+';
            //}
            else 
            {
                encoded << '%' << std::setw(2) << static_cast<unsigned int>(c);
            }
        }

        return encoded.str();
    }
    std::string UrlDecodeUtf8(std::string_view input)
    {
        std::ostringstream decoded;

        for (uint64_t i = 0; i < input.size(); ++i) 
        {
            if (input[i] == '%') 
            {
                std::string_view hex = input.substr(i + 1, 2);
                unsigned char decode_char = std::stoi(hex.data(), nullptr, 16);

                decoded << decode_char;
                i += 2; // Skip the two hexadecimal characters
            } 
            else if (input[i] == '+') 
            {
                decoded << ' '; // Replace '+' with space
            } 
            else 
            {
                decoded << input[i];
            }
        }

        return decoded.str();
    }

    HttpQuery::HttpQuery() {}
    HttpQuery::HttpQuery(std::string_view query)
    {
        if (query.empty())
            return;

        std::string decoded = UrlDecodeUtf8(query);

        if (decoded.front() == '?')
            decoded.erase(0, 1);

        SetQuery(decoded);
    }

    void HttpQuery::SetQuery(std::string_view query)
    {
        if (query.empty()) 
            return;

        std::string_view view = query;

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
    
    std::vector<StringArg> HttpQuery::ToArgs(std::string_view arg_format) const
    {
        std::vector<StringArg> args;

        if (!IsValid())
            return args;

        args.reserve(Size());

        std::transform(
            args_.cbegin(),
            args_.cend(),
            std::back_inserter(args), 
            [&arg_format](auto const& pair) 
            {
                return StringArg(pair.first, pair.second, arg_format); 
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