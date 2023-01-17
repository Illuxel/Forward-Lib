#include "fl/utils/StringBuilder.hpp"

#include <algorithm>
#include <iostream>

#include <fstream>

namespace fl::utils {
    
    StringArg::StringArg(std::string_view arg, std::string_view data, const char& format) 
        : m_ArgFormat(format)
        , m_ArgSpecifier(arg)
        , m_Data(data) 
    {
        m_Data.shrink_to_fit();
        m_ArgSpecifier.shrink_to_fit();
    }

    StringArg::StringArg()
        : m_ArgFormat('%') {}

    StringArg::StringArg(StringArg&& right)
        : m_ArgFormat(std::move(right.format()))
        , m_ArgSpecifier(std::move(right.specifier()))
        , m_Data(std::move(right.data())) {}

    StringArg::StringArg(const StringArg& right) 
        : m_ArgFormat(right.format())
        , m_ArgSpecifier(right.specifier())
        , m_Data(right.data()) {}

    StringArg::~StringArg() {}

    void StringArg::SetArgFormat(const char& format) 
    {
        m_ArgFormat = format;
    }
    void StringArg::SetArgSpecifier(std::string_view arg)
    {
        m_ArgSpecifier = arg;
    }
    
    void StringArg::SetArgData(std::string_view data) 
    {
        m_Data = data;
    }
    
    const char& StringArg::format() const
    {
        return m_ArgFormat;
    }
    std::string_view StringArg::specifier() const
    {
        return m_ArgSpecifier;
    }
    std::string StringArg::arg() const
    {
        return m_ArgFormat + m_ArgSpecifier;
    }

    std::string_view StringArg::data() const
    {
        return m_Data;
    }

    size_t StringArg::size() const 
    {
        return m_ArgSpecifier.size() + 1;
    }

    bool StringArg::IsDataEmpty() const 
    {
        return m_Data.empty();
    }


    StringBuilder::StringBuilder(std::string_view str, const ArgList& args)
        : m_StrBuild(str) 
    {
        this->BuildString(args);
    }

    StringBuilder::StringBuilder(const StringBuilder& right) 
        : m_StrBuild(right.m_StrBuild) {}

    StringBuilder::StringBuilder() {}
    StringBuilder::~StringBuilder() 
    {
        this->Clear();
    }

    void StringBuilder::SetTemplate(std::string_view str)
    {
        m_StrBuild = str;
    }

    void StringBuilder::BuildString(const StringArg& arg)
    {
        if (m_StrBuild.empty())
            return;

        size_t ArgPos{};

        while (true) 
        {
            ArgPos = m_StrBuild.find(arg.arg());

            if (ArgPos == std::string::npos)
                break;

            auto begin = m_StrBuild.cbegin() + ArgPos;
            auto end = m_StrBuild.cbegin() + ArgPos + arg.size(); 

            m_StrBuild.replace(begin, end, arg.data());
        }
    }
    void StringBuilder::BuildString(const ArgList& args) 
    {
        for (auto& arg : args)
            this->BuildString(arg);
    }

    StringBuilder& StringBuilder::Arg(const StringArg& arg) 
    {
        this->BuildString(arg);
        return *this;
    }
    StringBuilder& StringBuilder::Arg(const ArgList& args)
    {
        this->BuildString({args.cbegin(), args.cend()});
        return *this;
    }

    StringBuilder StringBuilder::FromFile(std::string_view fileName)
    {
        StringBuilder build;

        std::ifstream file(fileName.data());
        if (!file.is_open())
            return build;
 
        std::string data, temp;

        while (std::getline(file, temp))
        {
            data.append(temp + "\r\n");
        }

        file.close();
        build.SetTemplate(data);

        return build;
    }
    StringBuilder StringBuilder::FromFile(std::string_view fileName, const ArgList &args)
    {
        return StringBuilder::FromFile(fileName).Arg(args);
    }

    void StringBuilder::Clear() 
    {
        m_StrBuild.clear();
    }

    StringBuilder &StringBuilder::operator=(const char *arr)
    {
        SetTemplate(arr);
        return *this;
    }
    StringBuilder &StringBuilder::operator=(std::string_view str)
    {
        SetTemplate(str);
        return *this;
    }
}