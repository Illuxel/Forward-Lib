#pragma once

#include "fl/utils/Log.hpp"

#include "fl/utils/Memory.hpp"
#include "fl/utils/Exception.hpp"
#include "fl/utils/DateTime.hpp"

#include <shared_mutex>

#include <mysql_driver.h>
#include <mysql_connection.h>

#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/variant.h>

namespace Forward {

    class Database 
    {
    private:
        mutable std::shared_mutex db_mutex_;

        sql::mysql::MySQL_Driver* driver_;
        Scope<sql::Connection> connection_;

        bool is_scheme;

    public:
        virtual ~Database();

        bool Connect(std::string_view host, std::string_view user, std::string_view password);
        bool Connect(std::string_view host, std::string_view user, std::string_view password, Exception& ec);

        Scope<sql::ResultSet> Execute(std::string_view query) const;
        Scope<sql::ResultSet> Execute(std::string_view query, Exception& ec) const;

        template<typename ...Args>
        Scope<sql::ResultSet> Execute(std::string_view query, Args&&... args)
        {
            Exception ec;
            return std::move(Execute(query, ec, std::forward<Args>(args)...));
        }
        template<typename ...Args>
        Scope<sql::ResultSet> Execute(std::string_view query, Exception& ec, Args&&... args)
        {
            if (!IsConnected())
            {
                FL_LOG("Database", "is not connected");
                return nullptr;
            }
            if (!IsActiveSchema())
            {
                FL_LOG("Database", "scheme was not set");
                return nullptr;
            }

            std::unique_lock lock(db_mutex_);

            try
            {
                int index = 1;
                Scope<sql::PreparedStatement> statement(connection_->prepareStatement(query.data()));

                (BindValue(std::move(statement), index++, ec, std::forward<Args>(args)), ...);

                Scope<sql::ResultSet> result(statement->executeQuery());

                return std::move(result);
            }
            catch(Exception const& e)
            {
                ec = e;
            }

            return nullptr;
        }
        
        /**
         *  Closes current database connection
         */
        void Close();

        void SetActiveSchema(std::string_view scheme);

        bool IsConnected() const; 
        bool IsActiveSchema() const;

    private:
        static std::shared_mutex s_dbs_mutex;
        static std::unordered_map<std::string, Ref<Database>> s_dbs;

    public:
        /**
         *  Initializes database instance 
         * 
         *  @param db_name database tag
         *  @return reference to a database instance. If 
         */
        static Ref<Database> Init(std::string_view db_name);
        /**
         *  Gets database instance by tag. If no instance, will return nullptr
         * 
         *  @param db_name database tag
         *  @return reference to a database instance
         */
        static Ref<Database> Get(std::string_view db_name);
        /**
         *  Closes all connections and removes database instance by tag
         *  
         *  @param db_name database tag
         */
        static void Remove(std::string_view db_name);
        /**
         *  Checks for existing instance
         *  
         *  @param db_name database tag
         *  @return true if connection exist
         */
        static bool HasDatabase(std::string_view db_name);

        Database(Database const&) = delete;
        Database& operator=(Database const&) = delete;

    private:
        explicit Database(sql::mysql::MySQL_Driver* driver);

        template<typename T>
        void BindValue(Scope<sql::PreparedStatement> const& statement, const uint8_t index, Exception& ec, const T value) 
        {
            try
            {
                BindValueImpl(std::move(statement), index, value);
            }
            catch(Exception const& e)
            {
                ec = e;
            }
        }
    };

    void BindValueImpl(Scope<sql::PreparedStatement> const& statement, const uint8_t index, bool value);
    void BindValueImpl(Scope<sql::PreparedStatement> const& statement, const uint8_t index, int32_t value);
    void BindValueImpl(Scope<sql::PreparedStatement> const& statement, const uint8_t index, uint32_t value);
    void BindValueImpl(Scope<sql::PreparedStatement> const& statement, const uint8_t index, int64_t value);
    void BindValueImpl(Scope<sql::PreparedStatement> const& statement, const uint8_t index, uint64_t value);
    void BindValueImpl(Scope<sql::PreparedStatement> const& statement, const uint8_t index, double value);
    void BindValueImpl(Scope<sql::PreparedStatement> const& statement, const uint8_t index, const char* value);
    void BindValueImpl(Scope<sql::PreparedStatement> const& statement, const uint8_t index, std::string_view value);
    void BindValueImpl(Scope<sql::PreparedStatement> const& statement, const uint8_t index, DateTime const& date);
}
