#pragma once

#include "fl/web/HttpClient.hpp"

#include "fl/forward/Account.hpp"

#include "fl/forward/Chat.hpp"
#include "fl/forward/Group.hpp"

#include "fl/forward/Message.hpp"
#include "fl/forward/ForwardMessage.hpp"

namespace Forward::App {

    class Client
    {
        Scope<Web::HttpClient> sender_;

    public:
        Client();
        ~Client();

        bool Login(std::string_view email, std::string_view password);
        bool Register(std::string_view username, std::string_view email, std::string_view password);

        API::Account GetAccount() const;
        std::vector<API::Account> GetAccounts() const;

        std::vector<API::Chat> GetChats() const;
        std::vector<API::Group> GetGroups() const;

        void CreateGroup(std::string_view name) const;
    
        bool SendMessageTo(API::Message msg);
        bool SendMessageTo(std::string_view str);
    };
}