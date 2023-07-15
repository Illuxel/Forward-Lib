#pragma once

#include <string>
#include <optional>

#include <exception>

namespace Forward {
    
    class Exception : public std::exception
    {
    private:
        bool is_error;
        std::optional<std::string> msg_;

    public:
        Exception();
        
        explicit Exception(std::string_view msg);
        explicit Exception(std::exception const& ec);

        virtual ~Exception() override;

        virtual char const* what() const noexcept override
        {
            if (msg_.has_value())
                return msg_.value().c_str();
            return "Unknown exception";
        }

        operator bool() const 
        {
            return is_error;
        }
        operator const char*() const 
        {
            return what();
        }
        operator std::string() const 
        {
            return msg_.value_or("Unknown exception");
        }

        Exception& operator=(std::string_view msg);
        Exception& operator=(std::exception const& ec);
    };
} // namespace Forward
