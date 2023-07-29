#include "fl/utils/Exception.hpp"

namespace Forward {

    Exception::Exception()
        : std::exception{}
    {
    }

    Exception::Exception(std::string_view msg)
        : std::exception{}
    {
        msg_ = msg;
    }
    Exception::Exception(std::exception const& ec)
        : std::exception{}
    {
        SetError(ec);
    }

    Exception::Exception(Exception&& right) noexcept
        : std::exception(std::move(right))
    {
        is_error = std::move(right.is_error);
        msg_ = std::move(right.msg_);
    }
    Exception::Exception(Exception const& right) noexcept
        : std::exception(right)
    {
        is_error = right.is_error;
        msg_ = right.msg_;
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
