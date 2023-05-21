#pragma once

#include <optional>

#include <map>
#include <vector>
#include <string>

namespace fl::utils {

    class StringArg 
    {
        std::optional<char> specifier_;

        std::string name_;
        std::optional<std::string> arg_data_;

    public:
        // without specifier

        StringArg();
        StringArg(std::string_view name);
        StringArg(std::string_view name, std::string_view arg_data);

        // with specifier

        StringArg(char specifier);
        StringArg(std::string_view name, char specifier);
        StringArg(std::string_view name, std::string_view arg_data, char specifier);

        void SetSpecifier(char specifier);
        void SetName(std::string_view name);

        void SetData(std::string_view arg_data);

        std::optional<char> Specifier() const;
        std::string_view Name() const;
        std::string Joined() const;

        std::string_view Data() const;

        bool HasSpecifier() const;

        static std::vector<StringArg> 
        MakeArgs(std::initializer_list<char> const& args, 
                bool use_specifier = false, 
                char specifier = '%');

        static std::vector<StringArg> 
        MakeArgs(std::map<std::string, std::string> const& mapped_args, 
                bool use_specifier = false, 
                char specifier = '%');

        StringArg& operator=(StringArg const& right);

        bool operator==(StringArg const& arg) const;
        bool operator==(std::string_view data) const;

        operator std::string() const 
        {
            if (specifier_.has_value())
                return specifier_.value() + name_;

            return name_;
        }
    };

    class StringBuilder
    {
        std::optional<std::string> result_;

    public:
        StringBuilder();
        StringBuilder(std::string_view str, std::vector<StringArg> const& args = {});

        std::string Data() const;
        void SetTemplate(std::string_view str);

        StringBuilder& Arg(StringArg const& arg);
        StringBuilder& Arg(std::vector<StringArg> const& args);

        static StringBuilder FromFile(std::string_view file_name);
        static StringBuilder FromFile(std::string_view file_name, std::vector<StringArg> const& args);

        void Clear();

        StringBuilder& operator=(char const* str_arr);
        StringBuilder& operator=(std::string_view str);

        operator char const*() const 
        {   
            if (result_.has_value())
               return result_.value().c_str();

            return "";
        }
        operator std::string() const 
        {
            if (result_.has_value())
               return result_.value().c_str();

            return "";
        }

    private:
        void BuildString(StringArg const& arg);
        void BuildString(std::vector<StringArg> const& args);
    };
}