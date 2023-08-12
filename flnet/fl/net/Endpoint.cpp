#include "fl/net/Endpoint.hpp"
#include "fl/utils/Exception.hpp"

namespace Forward::Net {
    
    Endpoint::Endpoint() 
    {
        endpoint_ = { Core::Ip::address_v4(), 8080 };
        is_valid = true;
    }
    Endpoint::Endpoint(std::string_view str) 
    {
        uint64_t delim = str.find(':');

        if (delim == std::string::npos)
            return;

        std::string_view ip = str.substr(0, delim);
        std::string_view port_str = str.substr(delim + 1, str.size());

        uint16_t port = static_cast<uint16_t>(std::atoi(port_str.data()));

        endpoint_ = { Core::Ip::make_address(ip), port };
        is_valid = true;
    }
    Endpoint::Endpoint(std::string_view ip, uint16_t port)
    {
        endpoint_ = { Core::Ip::make_address(ip), port };
        is_valid = true;
    }

    std::string Endpoint::Address() const 
    {
        return endpoint_.address().to_string();
    }
    uint16_t Endpoint::Port() const 
    {
        return endpoint_.port();
    }

    Core::Tcp Endpoint::Protocol() const
    {
        return endpoint_.protocol();
    }

    Endpoint::operator Core::Tcp::endpoint const&() const&
    {
        return endpoint_;
    }
} // namespace

