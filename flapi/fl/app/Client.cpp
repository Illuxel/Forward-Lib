#include "fl/app/Client.hpp"

namespace Forward::App {

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

    API::Account Client::GetAccount() const 
    {
        return API::Account(); 
    }
    std::vector<API::Account> Client::GetAccounts() const
    {
        return std::vector<API::Account>();
    }

    std::vector<API::Chat> Client::GetChats() const
    {
        return std::vector<API::Chat>();
    }
    std::vector<API::Group> Client::GetGroups() const 
    {
        return std::vector<API::Group>();
    }

    void Client::CreateGroup(std::string_view name) const
    {
    }

    bool Client::SendMessageTo(API::Message msg)
    {
        return false;
    }
    bool Client::SendMessageTo(std::string_view str)
    {
        return false;
    }
}