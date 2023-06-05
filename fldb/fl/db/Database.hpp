#pragma once

#include "fl/utils/Memory.hpp"

#include <mysql/jdbc.h>

namespace fl::db {

    class Database
    {
        sql::mysql::MySQL_Driver* driver_;
        sql::Connection* connection_;

    private:
        static std::unordered_map<std::string, Ref<Database>> databases_;

        Database(sql::mysql::MySQL_Driver* driver);

    public:
        ~Database();

        static Ref<Database> Innit(std::string_view db_name);
        static Ref<Database> Get(std::string_view db_name);
        static void Remove(std::string_view db_name);

        static bool HasDatabase(std::string_view db_name);

        bool Connect(sql::ConnectOptionsMap options);
        bool Connect(std::string_view host, 
                    std::string_view user, 
                    std::string_view password);
        void Disconnect() const;

        bool IsConnected() const; 

        Database(const Database&) = delete;
        Database& operator=(const Database&) = delete;
    };
}