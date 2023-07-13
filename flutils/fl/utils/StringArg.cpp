#include "fl/utils/StringArg.hpp"

namespace Forward {
    
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
        specifier_.emplace(specifier);
    }
    void StringArg::SetName(std::string_view name) 
    {
        name_ = name;
    }
    void StringArg::SetData(std::string_view arg_data) 
    {
        arg_data_.emplace(arg_data);
    }

    std::optional<char> StringArg::Specifier() const
    {
        return specifier_;
    }
    std::string StringArg::Name() const
    {
        return name_;
    }
    std::string StringArg::Joined() const
    {
        if (specifier_.has_value())
            return specifier_.value() + name_;

        return name_;
    }

    std::string StringArg::Data() const
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

    StringArg StringArg::FromString(std::string_view str, char delim, 
                bool use_specifier, char specifier) 
    {
        auto const pos = str.find(delim);
        
        if (pos == std::string::npos)
            return StringArg();

        std::string_view arg_name = str.substr(0, pos), 
                        arg_data = str.substr(pos + 1, str.size());
        
        StringArg arg(arg_name, arg_data);

        if (use_specifier) 
            arg.SetSpecifier(specifier);

        return arg;
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
} // namespace Forward::util 
