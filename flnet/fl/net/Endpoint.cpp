#include "fl/net/Endpoint.hpp"
#include "fl/utils/Exception.hpp"

namespace Forward {
    
    Endpoint::Endpoint() 
    {
        endpoint_ = {net::ip::address_v4(), 80};
    }
    Endpoint::Endpoint(std::string_view str) 
    {
        size_t const it = str.find(':');

        if (it == std::string::npos)
            throw Exception("Failed to parse address");

        std::string_view ip = str.substr(0, it);

        std::string_view const port_str = str.substr(it + 1, str.size());
        uint16_t const port = static_cast<uint16_t>(std::atoi(port_str.data()));

        endpoint_ = {net::ip::make_address(ip), port};
    }
    Endpoint::Endpoint(std::string_view ip, uint16_t port)
    {
        endpoint_ = {net::ip::make_address(ip), port};
    }

    std::string Endpoint::Address() const 
    {
        return endpoint_.address().to_string();
    }
    uint16_t Endpoint::Port() const 
    {
        return endpoint_.port();
    }

    net::ip::tcp Endpoint::Protocol() const 
    {
        return endpoint_.protocol();
    }

    Endpoint::operator net::ip::tcp::endpoint const&() const&
    {
        return endpoint_;
    }
} // namespace

