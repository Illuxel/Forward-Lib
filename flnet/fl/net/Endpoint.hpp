#pragma once

#include "fl/net/Core.hpp"

namespace Forward {
    
    class Endpoint
    {
    private:
        tcp::endpoint endpoint_;

    public:
        Endpoint();
        Endpoint(std::string_view str);
        Endpoint(std::string_view ip, uint16_t port);

        std::string Address() const;
        uint16_t Port() const;

        net::ip::tcp Protocol() const;

        operator net::ip::tcp::endpoint() const;
    };
    
} // namespace Forward
