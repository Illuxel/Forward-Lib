#include "fl/net/Endpoint.hpp"

namespace fl {
    
    Endpoint::Endpoint() 
    {
        m_Endpoint = {net::ip::address_v4(), 8080};
    }
    Endpoint::Endpoint(std::string_view str) 
    {
        auto const it = str.find(':');

        auto const ip = str.substr(0, it);

        auto const portStr = str.substr(it + 1, str.size());
        auto const port = static_cast<unsigned short>(std::atoi(portStr.data()));

        m_Endpoint = {net::ip::make_address(ip), port};
    }
    Endpoint::Endpoint(std::string_view ip, unsigned short port) 
    {
        m_Endpoint = {net::ip::make_address(ip), port};
    }

    std::string Endpoint::Address() const 
    {
        return m_Endpoint.address().to_string();
    }
    unsigned short Endpoint::Port() const 
    {
        return m_Endpoint.port();
    }

    net::ip::tcp Endpoint::Protocol() const 
    {
        return m_Endpoint.protocol();
    }

    Endpoint::operator net::ip::tcp::endpoint() const
    {
        return m_Endpoint;
    }
} // namespace

