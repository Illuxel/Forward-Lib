#pragma once

#include "fl/web/HttpClient.hpp"

#include "fl/forward/Account.hpp"

#include "fl/forward/Message.hpp"
#include "fl/forward/ForwardMessage.hpp"

#include "fl/forward/Chat.hpp"
#include "fl/forward/Group.hpp"

namespace Forward::API {

    class Client
    {
        Ref<Web::HttpClient> sender_;

    public:
        Client();
        ~Client();

        bool Login(std::string_view email, std::string_view password);
        bool Register(std::string_view username, std::string_view email, std::string_view password);

        std::vector<Data::Account> GetAccount() const;

        std::vector<Data::Chat> GetChats() const;
        std::vector<Data::Group> GetGroups() const;

        void CreateGroup(std::string_view name) const;
    
        bool SendMessage(Data::Message msg);
        bool SendMessage(std::string_view str);
    };
}