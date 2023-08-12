#include "fl/forward/Client.hpp"

namespace Forward::API {

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
    std::vector<Chat> Client::GetAccount() const
    {
        return std::vector<Chat>();
    }
    std::vector<Chat> Client::GetChats() const
    {
        return std::vector<Chat>();
    }
    void Client::CreateGroup(std::string_view name) const
    {
    }
    bool Client::SendMessage(json::value json) const
    {
        return false;
    }
    bool Client::SendMessage(std::string_view str) const
    {
        return false;
    }
}