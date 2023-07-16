#pragma once

#include "fl/utils/Log.hpp"

#include "fl/utils/Memory.hpp"
#include "fl/utils/Exception.hpp"
#include "fl/utils/DateTime.hpp"

#include <mysql/jdbc.h>

namespace Forward {

    class DBConnection 
    {
    private:
        mutable std::mutex conn_mtx_;           // for connection ptr ONLY

        sql::Driver* driver_;
        Scope<sql::Connection> connection_;

        mutable std::shared_mutex data_mtx_;    // for everything else

        bool is_scheme;

    public:
        /**
         *   @param driver MySQL driver instance
         */
        DBConnection(sql::Driver* driver);
        /**
         *   Takes ownership of scope connection
         * 
         *   @param connection
         */
        DBConnection(sql::Connection* connection);
        /**
         *   Takes ownership of scope connection
         * 
         *   @param connection 
         */
        DBConnection(Scope<sql::Connection>&& connection);
        /**
         *   
         * 
         *   @param driver MySQL driver instance
         *   @param conn_options is a map with properties: https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-connect-options.html
         */
        DBConnection(sql::Driver* driver, sql::ConnectOptionsMap const& conn_options);
        /**
         *
         *
         *   @param driver MySQL driver instance
         *   @param conn_options is a map with properties: https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-connect-options.html
         */
        DBConnection(sql::Driver* driver, std::string_view host, std::string_view user, std::string_view password);

        virtual ~DBConnection();

        /**
         *  Sets current database scheme/name
         * 
         *  @param scheme database scheme/name
         */
        void SetActiveSchema(std::string_view scheme);

        /**
         *  Connects to existing database using options map
         *
         *  @param conn_options is a map with properties: https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-connect-options.html
         *  @param ec callback error
         *
         *  @return true if connection success
         */
        bool Connect(sql::ConnectOptionsMap const& conn_options);
        /**
         *  Connects to existing database using options map.
         *
         *  @param conn_options is a map with properties: https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-connect-options.html
         *  @param ec callback error
         *
         *  @return true if connection success
         */
        bool Connect(sql::ConnectOptionsMap const& conn_options, Exception& ec);
        /**
         *  Connects to existing database. Handles exceptions in method scope 
         * 
         *  @param host     
         *  @param user
         *  @param password
         * 
         *  @return true if connection success
         */
        bool Connect(std::string_view host, std::string_view user, std::string_view password);
        /**
         *  Connects to existing database.
         * 
         *  @param host
         *  @param user
         *  @param password
         *  @param ec callback error
         * 
         *  @return true if connection success
         */
        bool Connect(std::string_view host, std::string_view user, std::string_view password, Exception& ec);

        /**
         *  Executes query. Handles exceptions in method scope 
         * 
         *  @param query SQL query
         * 
         *  @return query result or nullptr 
         */
        Scope<sql::ResultSet> Execute(std::string_view query);
        /**
         *  Executes query.
         * 
         *  @param query SQL query
         *  @param ec callback error
         * 
         *  @return query result or nullptr 
         */
        Scope<sql::ResultSet> Execute(std::string_view query, Exception& ec);
        /**
         *  Executes query with arguments. Handles exceptions in method scope 
         * 
         *  @param query    SQL query
         *  @param args     argument list for query parameters. 
         *                  Each argument represents '?' in SQL query
         * 
         *  @return query result or nullptr 
         */
        template<typename ...Args>
        Scope<sql::ResultSet> Execute(std::string_view query, Args&&... args)
        {
            Exception ec;
            auto result = Execute(query, ec, std::forward<Args>(args)...);

            return std::move(result);
        }
        /**
         *  Executes query with arguments.
         * 
         *  @param query    SQL query
         *  @param args     argument list for query parameters. 
         *                  Each argument represents '?' in SQL query
         *  @param ec callback error
         * 
         *  @return query result or nullptr 
         */
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

            std::lock_guard lock(conn_mtx_);

            try
            {
                uint32_t index = 1;
                Scope<sql::PreparedStatement> statement(connection_->prepareStatement(query.data()));

                (BindValue(statement.get(), index++, std::forward<Args>(args)), ...);

                Scope<sql::ResultSet> result(statement->executeQuery());

                return std::move(result);
            }
            catch (sql::SQLException const& e)
            {
                ec = e;
                FL_LOG("Database", ec);
            }
            catch (std::runtime_error const& e)
            {
                ec = e;
                FL_LOG("Database", ec);
            }

            return nullptr;
        }

        /**
         *  Closes database connection
         */
        void Close();

        /**
         *  Checks if database has  connection  
         *
         *  @return true if connection ptr is initialized, otherwise will return false
         */
        bool IsValid() const;
        /**
         *  Checks if database has established connection
         * 
         *  @return true if connection exist, otherwise false
         */
        bool IsConnected() const; 
        /**
         *  Checks if database scheme/name is set
         * 
         *  @return true if scheme is set, otherwise false
         */
        bool IsActiveSchema() const;

    private:

        template<typename T>
        void BindValue(sql::PreparedStatement* statement, uint32_t const index, const T value)
        {
            try
            {
                BindValueImpl(statement, index, value);
            }
            catch (std::exception const& e)
            {
                FL_LOG("BindValue", e.what());
            }
        }

        void BindValueImpl(sql::PreparedStatement* statement, uint32_t const index, bool value);
        void BindValueImpl(sql::PreparedStatement* statement, uint32_t const index, int32_t value);
        void BindValueImpl(sql::PreparedStatement* statement, uint32_t const index, uint32_t value);
        void BindValueImpl(sql::PreparedStatement* statement, uint32_t const index, int64_t value);
        void BindValueImpl(sql::PreparedStatement* statement, uint32_t const index, uint64_t value);
        void BindValueImpl(sql::PreparedStatement* statement, uint32_t const index, double value);
        void BindValueImpl(sql::PreparedStatement* statement, uint32_t const index, const char* value);
        void BindValueImpl(sql::PreparedStatement* statement, uint32_t const index, std::string_view value);
        void BindValueImpl(sql::PreparedStatement* statement, uint32_t const index, DateTime const& date);
    };    
} // namespace Forward
