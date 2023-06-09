#pragma once

#include "fl/utils/Memory.hpp"
#include "fl/utils/DateTime.hpp"
#include "fl/utils/Log.hpp"

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>

#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>

namespace fl::db {

    class Database final
    {
        sql::mysql::MySQL_Driver* driver_;
        Scope<sql::Connection> connection_;
        bool is_scheme;

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
        void Close() const;

        void SetActiveSchema(std::string_view scheme);

        Ref<sql::ResultSet> Execute(std::string_view query) const;

        template<typename ...Args>
        Ref<sql::ResultSet> PrepareQuery(std::string_view query, Args&&... args)
        {
            Ref<sql::ResultSet> result;

            if (!IsConnected())
            {
                FL_LOG("Database", "is not connected");
                return result;
            }
            if (!is_scheme)
            {
                FL_LOG("Database", "scheme was not set");
                return result;
            }

            Ref<sql::PreparedStatement> statement(connection_->prepareStatement(query.data()));

            int index = 1;
            (BindValue(statement, index++, std::forward<Args>(args)), ...);

            try
            {
                Scope<sql::ResultSet> set(statement->executeQuery());
                result = std::move(set);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }

            return result;
        }

        bool IsConnected() const; 

        Database(const Database&) = delete;
        Database& operator=(const Database&) = delete;

    private:
        template<typename T>
        void BindValue(Ref<sql::PreparedStatement> const& statement, int index, T&& value)
        {
            BindValueImpl(statement, index, std::forward<T>(value));
        }

        void BindValueImpl(Ref<sql::PreparedStatement> const& statement, int index, bool value);
        void BindValueImpl(Ref<sql::PreparedStatement> const& statement, int index, int32_t value);
        void BindValueImpl(Ref<sql::PreparedStatement> const& statement, int index, uint32_t value);
        void BindValueImpl(Ref<sql::PreparedStatement> const& statement, int index, int64_t value);
        void BindValueImpl(Ref<sql::PreparedStatement> const& statement, int index, uint64_t value);
        void BindValueImpl(Ref<sql::PreparedStatement> const& statement, int index, double value);
        void BindValueImpl(Ref<sql::PreparedStatement> const& statement, int index, const char* value);
        void BindValueImpl(Ref<sql::PreparedStatement> const& statement, int index, std::string_view value);
        void BindValueImpl(Ref<sql::PreparedStatement> const& statement, int index, DateTime const& date);
    };
}