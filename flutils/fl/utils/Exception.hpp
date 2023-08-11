#pragma once

#include <exception>
#include <shared_mutex>

namespace Forward {
    
    /**
     * Native thread safe exception class 
     */
    class Exception : public std::exception
    {
    private:
        std::string msg_;
        bool is_error = false;

        mutable std::shared_mutex mtx_;

    public:
        Exception();
        Exception(std::string_view msg);
        Exception(std::exception const& ec);

        Exception(Exception&& ec) noexcept;
        Exception(Exception const& ec) noexcept;

        virtual ~Exception() override;

        void SetError(std::exception const& ec);

        bool IsError() const;

        virtual char const* what() const noexcept override;

        Exception& operator=(std::string_view msg) noexcept;
        Exception& operator=(std::exception const& ec) noexcept;

        explicit operator bool() const noexcept
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
            return os << ec.what();
        }
    };
} // namespace Forward
