#include "fl/utils/StringBuilder.hpp"

#include <algorithm>
#include <iostream>

#include <fstream>

namespace Forward {
    
    StringBuilder::StringBuilder() {}
    {
        result_.emplace(templ);
    StringBuilder::StringBuilder(std::string_view str, StringArgList const& args)
        BuildString(args);
    }

    void StringBuilder::SetTemplate(std::string_view str)
    {
        if (str.empty())
            return;

        result_.emplace(str);
    }
    {
        result_.emplace(templ);
    }

    StringBuilder& StringBuilder::Arg(StringArg const& arg) 
    {
        BuildString(arg);
        return *this;
    }
    StringBuilder& StringBuilder::Arg(StringArgList const& args)
    {
        BuildString(args);
        return *this;
    }

    StringArg StringBuilder::AsArg(std::string_view name) const 
    {
        return StringArg(name, result_.value_or(""));
    }
    StringArg StringBuilder::AsArg(std::string_view name, char specifier) const 
    {
        return StringArg(name, result_.value_or(""), specifier);
    }

    std::string StringBuilder::Data() &&
    {
        return std::move(result_.value_or(""));
    }
    std::string StringBuilder::Data() const &
    {
        return result_.value_or("");
    }

    void StringBuilder::Clear() 
    {
        result_.reset();
    }

    bool StringBuilder::IsValid() const 
    {
        return result_.has_value();
    }

    StringBuilder StringBuilder::FromFile(std::string_view file_name)
    {
        StringBuilder build;
        std::ifstream file(file_name.data(), std::ios::in);
        
        if (!file.is_open())
            return build;
 
        std::string data{std::istreambuf_iterator<char>(file),
                              std::istreambuf_iterator<char>()};

        file.close();
        build.SetTemplate(data);

        return build;
    }
    StringBuilder StringBuilder::FromFile(std::string_view file_name, StringArgList const& args)
    {
        return StringBuilder::FromFile(file_name).Arg(args);
    }

    StringBuilder& StringBuilder::operator=(std::string_view templ)
    {
        SetTemplate(templ);
        return *this;
    }

    void StringBuilder::BuildString(StringArg const& arg)
    {
        if (!IsValid())
            return;

        std::string& result = result_.value();
        size_t arg_pos;

        while (true) 
        {
            arg_pos = result.find(arg.Joined());

            if (arg_pos == std::string::npos)
                break;

            auto const& begin = result.cbegin() + arg_pos;
            auto const& end = result.cbegin() + arg_pos + arg.Joined().size();

            result.replace(begin, end, arg.Data());
        }
    }
    void StringBuilder::BuildString(StringArgList const& args)
    {
        for (auto& arg : args)
            BuildString(arg);
    }
}