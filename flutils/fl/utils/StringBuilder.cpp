#include "fl/utils/StringBuilder.hpp"

#include <algorithm>
#include <iostream>

#include <fstream>

namespace fl::utils {
    
    StringBuilder::StringBuilder() {}
    StringBuilder::StringBuilder(std::string_view str, std::vector<StringArg> const& args) 
    {
        result_.emplace(str);
        BuildString(args);
    }

    std::string StringBuilder::Data() const
    {
        return result_.value_or("");
    }
    void StringBuilder::SetTemplate(std::string_view str)
    {
        result_.emplace(str);
    }

    StringBuilder& StringBuilder::Arg(StringArg const& arg) 
    {
        this->BuildString(arg);
        return *this;
    }
    StringBuilder& StringBuilder::Arg(std::vector<StringArg> const& args)
    {
        this->BuildString(args);
        return *this;
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
    StringBuilder StringBuilder::FromFile(std::string_view file_name, std::vector<StringArg> const& args)
    {
        return StringBuilder::FromFile(file_name).Arg(args);
    }

    void StringBuilder::Clear() 
    {
        if (!result_.has_value())
            return;

        result_.value().clear();
    }

    StringBuilder &StringBuilder::operator=(char const* arr)
    {
        SetTemplate(arr);
        return *this;
    }
    StringBuilder &StringBuilder::operator=(std::string_view str)
    {
        SetTemplate(str);
        return *this;
    }

    void StringBuilder::BuildString(StringArg const& arg)
    {
        if (!result_.has_value())
            return;

        std::string& result = result_.value();

        if (result.empty())
            return;

        size_t ArgPos;

        while (true) 
        {
            ArgPos = result.find(arg.Joined());

            if (ArgPos == std::string::npos)
                break;

            auto const& begin = result.cbegin() + ArgPos;
            auto const& end = result.cbegin() + ArgPos + arg.Joined().size(); 

            result.replace(begin, end, arg.Data());
        }
    }
    void StringBuilder::BuildString(std::vector<StringArg> const& args) 
    {
        for (auto& arg : args)
            this->BuildString(arg);
    }
}