#include "fl/forward/Client.hpp"

namespace fl {

    Client::Client()
    {
    }

    bool Client::Login(std::string_view email, std::string_view password) 
    {
        return true;
    }
    bool Client::Register(std::string_view username, std::string_view email, std::string_view password) 
    {
        return true;
    }
}