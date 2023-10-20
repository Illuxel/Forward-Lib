#include "fl/forward/Client.hpp"

namespace Forward::API {

    Client::Client()
    {
    }
    Client::~Client()
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
    std::vector<Data::Account> Client::GetAccount() const
    {
        return std::vector<Data::Account>();
    }
    std::vector<Data::Chat> Client::GetChats() const
    {
        return std::vector<Data::Chat>();
    }
    void Client::CreateGroup(std::string_view name) const
    {
    }

    bool Client::SendMessage(Data::Message msg)
    {
        return false;
    }
    bool Client::SendMessage(std::string_view str)
    {
        return false;
    }
}