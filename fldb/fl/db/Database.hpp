#pragma once

#include "fl/utils/Log.hpp"

#include "fl/utils/Memory.hpp"
#include "fl/utils/DateTime.hpp"

#include <mysql_driver.h>
#include <mysql_connection.h>

#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/variant.h>

namespace fl {

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
        bool Connect(sql::ConnectOptionsMap options, std::exception& ec);

        bool Connect(std::string_view host, std::string_view user, std::string_view password);
        bool Connect(std::string_view host, std::string_view user, std::string_view password, std::exception& ec);

        void Close() const;

        void SetActiveSchema(std::string_view scheme);

        Ref<sql::ResultSet> Execute(std::string_view query) const;
        Ref<sql::ResultSet> Execute(std::string_view query, std::exception& ex) const;

        template<typename ...Args>
        Ref<sql::ResultSet> PrepareQuery(std::string_view query, Args&&... args)
        {
            std::exception ec;
            return PrepareQuery(query, ec,  std::forward<Args>(args)...);
        }
        template<typename ...Args>
        Ref<sql::ResultSet> PrepareQuery(std::string_view query, std::exception& ec, Args&&... args)
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

            try
            {
                int index = 1;
                Scope<sql::PreparedStatement> statement(connection_->prepareStatement(query.data()));
                (BindValue(statement, index++, std::forward<Args>(args)), ...);

                Scope<sql::ResultSet> scope(statement->executeQuery());
                result = std::move(scope);
            }
            catch(std::exception const& e)
            {
                ec = e;
            }

            return result;
        }

        bool IsConnected() const; 
        bool IsActiveSchema() const;

        Database(const Database&) = delete;
        Database& operator=(const Database&) = delete;

    private:
        template<typename T>
        void BindValue(Scope<sql::PreparedStatement> const& statement, int index, T&& value)
        {
            BindValueImpl(statement, index, std::forward<T>(value));
        }

        void BindValueImpl(Scope<sql::PreparedStatement> const& statement, int index, bool value);
        void BindValueImpl(Scope<sql::PreparedStatement> const& statement, int index, int32_t value);
        void BindValueImpl(Scope<sql::PreparedStatement> const& statement, int index, uint32_t value);
        void BindValueImpl(Scope<sql::PreparedStatement> const& statement, int index, int64_t value);
        void BindValueImpl(Scope<sql::PreparedStatement> const& statement, int index, uint64_t value);
        void BindValueImpl(Scope<sql::PreparedStatement> const& statement, int index, double value);
        void BindValueImpl(Scope<sql::PreparedStatement> const& statement, int index, const char* value);
        void BindValueImpl(Scope<sql::PreparedStatement> const& statement, int index, std::string_view value);
        void BindValueImpl(Scope<sql::PreparedStatement> const& statement, int index, DateTime const& date);
    };
}
