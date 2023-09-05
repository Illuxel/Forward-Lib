#include "fl/utils/StringArgParser.hpp"

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
        uint64_t pos = str.find(sep);

        if (pos == std::string::npos)
            return StringArg();

        std::string_view arg_name = str.substr(0, pos);

        if (pos >= str.size())
            return StringArg(arg_name);

        std::string_view arg_value = str.substr(pos + 1, str.size());

        return StringArg(arg_name, arg_value.data());
    }

    std::vector<StringArg> StringArgParser::FromString(std::string_view str, char sep, char delim, std::string_view format)
    {
        std::vector<StringArg> arg_list;
        std::string_view view = str;

        while (uint64_t pos = view.find(delim) != std::string::npos)
        {
            std::string_view sub_str = view.substr(0, pos);

            StringArg arg = StringArgParser::FromString(sub_str, sep);

            if (!arg.IsValid())
                break;

            arg_list.emplace_back(arg);

            view = view.substr(0);
        }

        return arg_list;
    }
}
