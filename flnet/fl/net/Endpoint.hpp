#pragma once

#include "fl/net/Core.hpp"

namespace Forward::Net {
    
    class Endpoint
    {
    private:
        bool is_valid = false;
        Core::Tcp::endpoint endpoint_;

    public:
        Endpoint();
        Endpoint(std::string_view str);
        Endpoint(std::string_view ip, uint16_t port);

        std::string Address() const;
        uint16_t Port() const;

        Core::Tcp Protocol() const;

        bool IsValid() const { return is_valid; }

        operator Core::Tcp::endpoint const&() const&;
    };
    
} // namespace Forward
