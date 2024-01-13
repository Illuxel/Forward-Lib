#pragma once

#include "fl/web/Core.hpp"

namespace Forward::Web {

    class HttpQuery
    {
    private:
        bool is_valid = false;
        std::unordered_map<std::string, std::string> args_;

    public:
        HttpQuery();
        HttpQuery(std::string_view query);

        void SetQuery(std::string_view query);

        // Get/TryGet

        StringArg Arg(std::string_view key) const;
        std::string_view Value(std::string_view key) const;

        std::vector<std::string> Keys() const;

        std::string ToString() const;
        StringArgList ToArgs(std::string_view format = "") const;

        uint64_t Size() const;

        bool IsValid() const;
        bool HasKey(std::string_view key) const;

        std::string& operator[](std::string_view key)&;
    };
} // namespace Forward