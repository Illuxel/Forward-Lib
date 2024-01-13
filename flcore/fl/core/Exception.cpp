#include "fl/core/Exception.hpp"

namespace Forward {

    Exception::Exception() 
        : std::exception() 
    {
    }

    Exception::Exception(std::string_view msg) 
        : std::exception() 
    {
        SetError(msg);
    }

    Exception::~Exception() {}

    void Exception::SetError(std::string_view msg)
    {
        std::unique_lock lock(mtx_);

        isError = true;
        errorMessage = msg;
    }
    void Exception::SetError(std::exception const& ex)
    { 
        SetError(ex.what());
    }

    const char* Exception::what() const
    {
        std::shared_lock lock(mtx_);

        return errorMessage.c_str();
    }

    bool Exception::IsError() const
    {
        std::shared_lock lock(mtx_);

        return isError;
    }

    Exception& Exception::operator=(std::string_view msg)
    {
        SetError(msg);

        return *this;
    }
    //Exception& Exception::operator=(std::exception const& ex) 
    //{
    //    SetError(ex);

    //    return *this;
    //}

    std::istream& operator>>(std::istream& is, Exception&) 
    { 

        return is;
    }
    std::ostream& operator<<(std::ostream& os, Exception const&) 
    { 
        return os;
    }

}  // namespace Forward
