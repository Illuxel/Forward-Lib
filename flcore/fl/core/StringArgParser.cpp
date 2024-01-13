#include "fl/core/StringArgParser.hpp"

namespace Forward {

    StringArgParser::StringArgParser() {}
    StringArgParser::StringArgParser(std::string_view str)
        : str_(str) {}

    uint32_t StringArgParser::ArgCount() const
    {
        return 0;
    }

    StringArg StringArgParser::FromString(std::string_view str, char sep, std::string_view format)
    {
        const uint64_t pos = str.find(sep);

        if (pos == std::string::npos)
            return StringArg();

        const auto key = str.substr(0, pos);

        if (pos >= str.size())
            return StringArg(key);

        const auto value = str.substr(pos + 1, str.size());

        return StringArg(key, value.data());
    }

    std::vector<StringArg> StringArgParser::FromString(std::string_view str, char sep, char delim, std::string_view format)
    {
        std::vector<StringArg> arg_list;
        std::string_view view = str;

        while (const uint64_t pos = view.find(delim) != std::string::npos)
        {
            const auto arg_view = view.substr(0, pos);
            const auto arg = StringArgParser::FromString(arg_view, sep);

            if (!arg.IsValid())
                break;

            arg_list.emplace_back(arg);
            view = view.substr(0);
        }

        return arg_list;
    }
}
