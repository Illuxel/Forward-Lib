#include "fl/utils/Exception.hpp"
#include "Exception.hpp"

namespace Forward {

    Exception::Exception()
        : std::exception{}
        , is_error(false) {}
    Exception::Exception(std::string_view msg)
        : std::exception{}
        , is_error(true)
    {
        msg_.emplace(msg);
    }
    Exception::Exception(std::exception const& ec)
        : std::exception{}
        , is_error(true) 
    {
        msg_.emplace(ec.what());
    }

    Exception::~Exception() {}

    Exception& Exception::operator=(std::string_view msg)
    {
        is_error = true; 
        msg_.emplace(msg);
        return *this;
    }
    Exception& Exception::operator=(std::exception const &ec)
    {
        is_error = true;
        msg_.emplace(ec.what());
        return *this;
    }

} // namespace Forward
