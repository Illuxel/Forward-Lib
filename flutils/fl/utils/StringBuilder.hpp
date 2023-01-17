#pragma once

#include <QChar>
#include <QByteArray>

namespace fl::utils {

    // encapsulates argument 
    // and data paired with him

    class StringArg 
    {
    public:
        StringArg(std::string_view arg, std::string_view data, const char& format = '%');

        StringArg();
        StringArg(StringArg&& right);
        StringArg(const StringArg& arg);
        ~StringArg();
        /**
         *  Sets a prefix
         */
        void SetArgFormat(const char& format);
        /**
         *  Sets an argument name 
         */
        void SetArgSpecifier(std::string_view data);
        /**
         *  Sets a data to an argument 
         */
        void SetArgData(std::string_view data);

        const char& format() const;
        std::string_view specifier() const;

        std::string arg() const;
        std::string_view data() const;

        size_t size() const;

        bool IsDataEmpty() const;
    
        StringArg& operator=(const StringArg& right) 
        {
            m_ArgFormat = right.m_ArgFormat;
            m_ArgSpecifier = right.specifier();
            m_Data = right.data();
            return *this;
        }

        bool operator==(const StringArg& arg) const
        {
            return (this->format() == arg.format()) && (this->specifier() == arg.specifier());
        }

        bool operator==(std::string_view specifier) const
        {
            return specifier.find(this->arg()) != std::string::npos;
        }
        
    private:
        char m_ArgFormat;
        std::string m_ArgSpecifier, m_Data;
    };

    // input str = text %arg text %arg
    // set args and data to it
    // create string from args

    class StringBuilder
    {
    public:
        using ArgList = std::vector <StringArg>;

        StringBuilder(std::string_view str, const ArgList& args = {});

        StringBuilder();
        StringBuilder(const StringBuilder& right);
        ~StringBuilder();

        void SetTemplate(std::string_view str);

        StringBuilder& Arg(const StringArg& arg);
        StringBuilder& Arg(const ArgList& args);

        static StringBuilder FromFile(std::string_view fileName);
        static StringBuilder FromFile(std::string_view fileName, const ArgList& args);

        void Clear();

    private:
        void BuildString(const StringArg& arg);
        void BuildString(const ArgList& args);

    public:
        StringBuilder& operator=(const char* arr);
        StringBuilder& operator=(std::string_view str);

        operator const char*() const {
            return m_StrBuild.c_str();
        }
        operator std::string() const {
            return m_StrBuild;
        }
        operator std::string_view() const {
            return m_StrBuild;
        }

    private:
        std::string m_StrBuild;
    };
}