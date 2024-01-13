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
        bool isError = false;
        std::string errorMessage;

        mutable std::shared_mutex mtx_;

    public:
        Exception();
        Exception(std::string_view msg);

        virtual ~Exception() override;

        void SetError(std::string_view msg);
        void SetError(std::exception const& ex);

        bool IsError() const;

        virtual const char* what() const override;

        Exception& operator=(std::string_view msg);

        explicit operator bool() const
        {
            return IsError();
        }
        operator const char*() const
        {
            return what();
        }
        operator std::string() const
        {
            return what();
        }

        friend std::istream& operator>>(std::istream&, Exception&);
        friend std::ostream& operator<<(std::ostream&, Exception const&);
    };
} // namespace Forward
