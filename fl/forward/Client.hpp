#pragma once

#include "fl/net/http/HttpClient.hpp"

#include "fl/forward/Chat.hpp"

namespace Forward::API {

    class Client
    {
        Ref<Web::HttpClient> sender_;

    public:
        Client();

        bool Login(std::string_view email, std::string_view password);
        bool Register(std::string_view username, std::string_view email, std::string_view password);

        std::vector<Chat> GetAccount() const;
        std::vector<Chat> GetChats() const;

        void CreateGroup(std::string_view name) const;
    
        bool SendMessage(json::value json) const;
        bool SendMessage(std::string_view str) const;
    };
}