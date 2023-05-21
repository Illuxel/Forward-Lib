#pragma once

#include "fl/net/Base.hpp"

namespace fl {
    
    class Endpoint
    {
        tcp::endpoint m_Endpoint;

    public:
        Endpoint();
        Endpoint(std::string_view str);
        Endpoint(std::string_view ip, unsigned short port);

        std::string Address() const;
        unsigned short Port() const;

        net::ip::tcp Protocol() const;

        operator net::ip::tcp::endpoint() const;
    };
} // namespace fl
