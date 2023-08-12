#pragma once

#include "fl/net/http/Core.hpp"

namespace Forward::Web {

    std::string UrlEncodeUtf8(std::string_view input);
    std::string UrlDecodeUtf8(std::string_view input);

    class HttpQuery
    {
    private:
        bool is_valid = false;
        std::unordered_map<std::string, std::string> args_;

    public:
        HttpQuery();
        HttpQuery(std::string_view query);

        void SetQuery(std::string_view query);

        StringArg Arg(std::string_view key) const;
        std::string_view Value(std::string_view key) const;

        std::vector<std::string> Keys() const;

        std::string ToString() const;
        std::vector<StringArg> ToArgs(std::string_view arg_format = "") const;

        uint64_t Size() const;

        bool IsValid() const;
        bool HasKey(std::string_view key) const;

        std::string& operator[](std::string_view key)&;
    };
} // namespace Forward