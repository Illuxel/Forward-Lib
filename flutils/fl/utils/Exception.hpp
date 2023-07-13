#pragma once

#include <string>
#include <exception>

namespace Forward {
    
    class Exception
    {
    private:
        bool is_error;
        std::exception ec_;

    public:
        Exception();
        Exception(std::exception const& ec);
        ~Exception();

        operator bool() const {
            return is_error;
        }
        operator const char*() const {
            return ec_.what();
        }
        operator std::string_view() const {
            return ec_.what();
        }
        operator std::string const&() const {
            return ec_.what();
        }
        operator std::exception const&() const & {
            return ec_;
        }
    };
} // namespace Forward
