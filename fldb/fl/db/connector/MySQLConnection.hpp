#pragma once

#include "fl/db/Query.hpp"
#include "fl/db/PreparedQuery.hpp"
#include "fl/db/Result.hpp"

#include "fl/db/DBConnection.hpp"

#include <mysqlx/xdevapi.h>

namespace Forward::Database {

    class MySQLConnection
    {
    public:
        using Options = ConnetionOptions<MySQLConnection>;

        using Query = Query<MySQLConnection>;
        using PreparedQuery = PreparedQuery<MySQLConnection>;

        using Result = Result<MySQLConnection>;

    private:
        bool isConnected = false, isActiveTable;

        Exception ex_;
        Scope<mysqlx::Session> conn_ = nullptr;

        mutable std::shared_mutex mtx_;

    public:

        /**
         * Takes ownership of scope ptr connection
         *
         * @param connection
         */
        MySQLConnection(Scope<mysqlx::Session> connection);
        /**
         * Create connection object and connects to database using ConnectOptionsMap
         *
         * @param driver MySQL driver instance
         * @param options 
         */
        MySQLConnection(Options const& options);
        /**
         * Create connection object and connects to database
         *
         * @param host
         * @param user
         * @param password
         */
        MySQLConnection(std::string_view host, std::string_view user, std::string_view password);

        /**
         * Sets active/current database table
         *
         * @param table current database table
         */
        void SetActiveTable(std::string_view table);

        Exception GetException() const { return ex_; }

        /**
         * Connects to existing database using options map
         *
         * @param options 
         * @param 
         
         
         
         callback error
         *
         * @return true if connection success
         */
        bool Connect(Options const& options);
        /**
         * Connects to existing database using options map.
         *
         * @param  
         * @param ex callback error
         *
         * @return true if connection success
         */
        bool Connect(std::string_view host, std::string_view user, std::string_view password, Exception& ex);

        /**
         * Connects to database using options map
         *
         * @param options 
         *
         * @return true if connection success
         */
        std::future<bool> ConnectAsync(Options const& options);
        /**
         * Connects to existing database asynchronously
         *
         * @param host
         * @param user
         * @param password
         * 
         * @param ex callback error
         *
         * @return true if connection success
         */
        std::future<bool> ConnectAsync(std::string_view host, std::string_view user, std::string_view password);

        Result Execute(std::string_view sql);
        Result Execute(std::string_view sql, Exception& ex);

        bool Close();

        bool IsValid() const
        {
            return (bool)conn_;
        }
        bool IsActiveTable() const 
        { 
            return isActiveTable;
        }
        bool IsConnected() const
        { 
            return isConnected;
        }


    };
} // namespace Forward::Database