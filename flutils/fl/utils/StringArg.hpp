#pragma once

#include "fl/utils/TypelessArg.hpp"

#include <map>
#include <vector>

namespace Forward {
    
    class StringArg
    {
    private:
        std::string name_;
        std::optional<char> specifier_;

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

        StringArg(StringArg&& right);
        StringArg(StringArg const& right);

        /**
         * Sets prefix to arg name 
         *
         * @param specifier prefix symbol
         */
        void SetSpecifier(char specifier);
        /**
         * Sets argument name
         * 
         * @param name argument name
         */
        void SetName(std::string_view name);
        /**
         * Sets argument data 
         * 
         * @param arg_data argument name
         */
        void SetData(std::string_view arg_data);

        std::optional<char> Specifier() const;
        
        std::string Name() const;
        /**
         * Joins specifier (prefix) and arg name
         *
         * @return string specifier (prefix) appended with arg name
         */
        std::string Joined() const;

        /**
         * Joins specifier (prefix) and arg name
         *
         * @return string specifier (prefix) appended with arg name
         */
        std::string Data() const;

        bool HasSpecifier() const;

        static StringArg FromString(std::string_view str, char delim = '=', 
            bool use_specifier = false, char specifier = '%');

        static std::vector<StringArg>
        MakeArgs(std::initializer_list<char> const& args);
        static std::vector<StringArg> 
        MakeArgs(std::initializer_list<char> const& args, char specifier);

        static std::vector<StringArg>
        MakeArgs(std::map<std::string, std::string> const& mapped_args);
        static std::vector<StringArg> 
        MakeArgs(std::map<std::string, std::string> const& mapped_args, char specifier);

        StringArg& operator=(StringArg&& right);
        StringArg& operator=(StringArg const& right);

        bool operator==(std::string_view right) const;
        bool operator==(StringArg const& right) const;

        operator std::string() const &
        {
            if (specifier_.has_value())
                return specifier_.value() + name_;

            return name_;
        }
    };

} // namespace Forward::util 
