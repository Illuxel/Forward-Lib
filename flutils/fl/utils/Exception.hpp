#pragma once

#include <string>
#include <optional>

#include <exception>

#include <shared_mutex>

namespace Forward {
    
    /**
    *   Thread safe exception class 
    */

    class Exception : public std::exception
    {
    private:
        bool is_error;
        std::string msg_;
        mutable std::shared_mutex mtx_;

    public:
        Exception();
        explicit Exception(Exception const& ec);
        explicit Exception(std::string_view msg);
        explicit Exception(std::exception const& ec);

        virtual ~Exception() override;

        void SetError(std::exception const& ec);

        bool IsError() const;

        virtual char const* what() const noexcept override;

        Exception& operator=(std::string_view msg) noexcept;
        Exception& operator=(std::exception const& ec) noexcept;

        operator bool() const noexcept
        {
            return IsError();
        }
        operator const char*() const noexcept
        {
            return what();
        }
        operator std::string() const noexcept
        {
            return what();
        }

        template<class Ch, class Tr>
        friend std::basic_ostream<Ch, Tr>&
        operator<<(std::basic_ostream<Ch, Tr>& os, Exception const& ec)
        {
            std::shared_lock lock(mtx_);
            return os << ec.what();
        }
    };
} // namespace Forward
