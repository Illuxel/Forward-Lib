#include "fl/utils/Exception.hpp"
#include "Exception.hpp"

namespace fl {

    Exception::Exception()
        : is_error(false) {}
    Exception::Exception(std::exception const &ec)
        : ec_(ec) 
        , is_error(true) {}

    Exception::~Exception(){}

} // namespace fl
