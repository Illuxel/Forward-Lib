#include "fl/core/StringBuilder.hpp"

#include <algorithm>
#include <iostream>

#include <fstream>

namespace Forward {
    
    StringBuilder::StringBuilder() {}
    StringBuilder::StringBuilder(std::string_view str, StringArgList const& args)
    {
        SetTemplate(str);
        BuildString(args);
    }

    void StringBuilder::SetTemplate(std::string_view str)
    {
        if (str.empty())
            return;

        result_.emplace(str);
    }

    StringBuilder& StringBuilder::ArgFirst(StringArg const& arg)
    {


        return *this;
    }
    StringBuilder& StringBuilder::ArgFirst(StringArgList const& args)
    {


        return *this;
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

    std::string StringBuilder::Data() &&
    {
        return std::move(result_.value_or(""));
    }
    std::string const& StringBuilder::Data() const&
    {
        return result_.value();
    }

    void StringBuilder::Clear() 
    {
        result_.reset();
    }

    bool StringBuilder::IsValid() const 
    {
        return result_.has_value();
    }

    bool StringBuilder::IsArg(StringArg const& arg) const
    {
        return true;
    }
    bool StringBuilder::IsArgs(StringArgList const& args) const 
    {
        return true;
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

        uint64_t arg_pos;
        std::string& result = result_.value();

        while (true) 
        {
            arg_pos = result.find(arg.GetParsed());

            if (arg_pos == std::string::npos)
                break;

            auto const& begin = result.cbegin() + arg_pos;
            auto const& end = result.cbegin() + arg_pos + arg.GetParsed().size();

            result.replace(begin, end, arg.GetData());
        }
    }
    void StringBuilder::BuildString(StringArgList const& args)
    {
        for (auto& arg : args)
            BuildString(arg);
    }
}