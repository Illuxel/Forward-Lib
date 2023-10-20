#include "fl/net/Endpoint.hpp"
#include "fl/utils/Exception.hpp"

namespace Forward::Net {
    
    Endpoint::Endpoint() 
    {
<<<<<<< Updated upstream
        endpoint_ = { Core::IpAddressV4(), 8080 };
=======
        endpoint_ = { Core::Asio::address_v4(), 8080 };
>>>>>>> Stashed changes
        is_valid = true;
    }
    Endpoint::Endpoint(std::string_view str) 
    {
        auto delim = str.find(':');

        if (delim == std::string::npos || delim == str.size())
            return;

        auto ip = str.substr(0, delim);
        auto port_str = str.substr(delim + 1, str.size());

        auto port = static_cast<uint16_t>(std::atoi(port_str.data()));

<<<<<<< Updated upstream
        Core::ErrorCode ec;

        endpoint_ = { Core::MakeAddress(ip, ec), port };
        is_valid = (bool)ec;
    }
    Endpoint::Endpoint(std::string_view ip, uint16_t port)
    {
        Core::ErrorCode ec;

        endpoint_ = { Core::MakeAddress(ip, ec), port };
        is_valid = (bool)ec;
=======
        endpoint_ = { Core::Asio::make_address(ip), port };
        is_valid = true;
    }
    Endpoint::Endpoint(std::string_view ip, uint16_t port)
    {
        endpoint_ = { Core::Asio::make_address(ip), port };
        is_valid = true;
>>>>>>> Stashed changes
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
} // namespace

