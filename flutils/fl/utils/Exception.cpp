#include "fl/utils/Exception.hpp"

namespace Forward {

    Exception::Exception()
        : std::exception{}
        , is_error(false) 
    {
    }
    Exception::Exception(std::string_view msg)
        : std::exception{}
        , is_error(true)
    {
        msg_ = msg;
    }
    Exception::Exception(std::exception const& ec)
        : std::exception{}
    {
        SetError(ec);
    }
    Exception::Exception(Exception const& ec)
        : std::exception(ec)
        , is_error(ec.is_error)
    {
        msg_ = ec.msg_;
    }

    Exception::~Exception() {}

    void Exception::SetError(std::exception const& ec)
    {
        std::unique_lock lock(mtx_);

        msg_ = ec.what();
        is_error = true;
    }

    char const* Exception::what() const noexcept
    {
        std::shared_lock lock(mtx_);

        if (msg_.empty())
            return msg_.c_str();
        return "Unknown exception";
    }

    bool Exception::IsError() const
    {
        std::shared_lock lock(mtx_);
        return is_error;
    }

    Exception& Exception::operator=(std::string_view msg) noexcept
    {
        std::unique_lock lock(mtx_);

        is_error = true;        
        msg_ = msg;

        return *this;
    }
    Exception& Exception::operator=(std::exception const& ec) noexcept 
    {
        SetError(ec);

        return *this;
    }

} // namespace Forward
