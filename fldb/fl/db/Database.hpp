#pragma once

#include "fl/utils/Memory.hpp"

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>

#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>

namespace fl::db {

    class Database
    {
        Scope<sql::mysql::MySQL_Driver> driver_;
        Scope<sql::Connection> connection_;

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

        void SetActiveSchema(std::string_view scheme);

        Ref<sql::ResultSet> Execute(std::string_view query);
        // Ref<sql::PreparedStatement> Prepare(std::string_view query);

        bool IsConnected() const; 

        void Disconnect() const;

        Database(const Database&) = delete;
        Database& operator=(const Database&) = delete;
    };
}