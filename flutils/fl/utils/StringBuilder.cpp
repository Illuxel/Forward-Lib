#include "fl/utils/StringBuilder.hpp"

#include <algorithm>
#include <iostream>

#include <fstream>

namespace fl::utils {
    
    StringArg::StringArg() {}

    // without specifier

    StringArg::StringArg(std::string_view name) 
        : name_(name) {}
    StringArg::StringArg(
        std::string_view name, 
        std::string_view arg_data)
        : name_(name)
    {
        arg_data_.emplace(arg_data);
    }

    // with specifier

    StringArg::StringArg(char specifier) 
    {
        specifier_.emplace(specifier);
    }
    StringArg::StringArg(std::string_view name, char specifier)
        : name_(name)
    {
        specifier_.emplace(specifier);
    }
    StringArg::StringArg(
        std::string_view name, 
        std::string_view arg_data,
        char specifier)
        : name_(name)
    {
        specifier_.emplace(specifier);
        arg_data_.emplace(arg_data);
    }

    void StringArg::SetSpecifier(char specifier)
    {
        specifier_ = specifier;
    }
    void StringArg::SetName(std::string_view name) 
    {
        name_ = name;
    }
    void StringArg::SetData(std::string_view arg_data) 
    {
        arg_data_ = arg_data;
    }

    std::optional<char> StringArg::Specifier() const
    {
        return specifier_;
    }
    std::string_view StringArg::Name() const
    {
        return name_;
    }
    std::string StringArg::Joined() const
    {
        if (specifier_.has_value())
            return specifier_.value() + name_;

        return name_;
    }

    std::string_view StringArg::Data() const
    {
        return *arg_data_;
    }

    bool StringArg::HasSpecifier() const 
    {
        return specifier_.has_value();
    }

    StringArg& StringArg::operator=(StringArg const& right) 
    {
        specifier_ = right.specifier_;
        name_ = right.name_;
        arg_data_ = right.arg_data_;

        return *this;
    }
    
    bool StringArg::operator==(StringArg const& arg) const
    {
        return (specifier_ == arg.specifier_) 
            && (name_ == arg.name_);
    }
    bool StringArg::operator==(std::string_view data) const
    {
        return data.find(Joined()) != std::string::npos;
    }

    std::vector<StringArg> 
    StringArg::MakeArgs(std::initializer_list<char> const& args, 
                bool use_specifier, 
                char specifier)
    {
        std::vector<StringArg> arg_list;

        for (auto c : args)
        {
            if (use_specifier)
            {
                arg_list.emplace_back(std::string(1, c), specifier);
            } 
            else 
            {
                arg_list.emplace_back(std::string(1, c));
            }
        }

        return arg_list;
    }
    std::vector<StringArg> 
    StringArg::MakeArgs(std::map<std::string, std::string> const& mapped_args, 
                bool use_specifier, char specifier) 
    {
        std::vector<StringArg> arg_list;

        for (auto& [arg, data] : mapped_args)
        {
            if (use_specifier)
            {
                arg_list.emplace_back(arg, data, specifier);
            } 
            else 
            {
                arg_list.emplace_back(arg, data);
            }
        }

        return arg_list;
    }


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