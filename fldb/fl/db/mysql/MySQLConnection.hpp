#pragma once

#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>

#include <jdbc/cppconn/connection.h>

#include <jdbc/mysql_error.h>
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>

namespace Forward::Database::MySQL {

    class MySQLConnection 
    {
    private:
        sql::Driver* driver_ = nullptr;
        Scope<sql::Connection> conn_ = nullptr;

    public:
        /**
		 * @param driver MySQL driver instance
		 */
		MySQLConnection(sql::Driver* driver);
		/**
		 * Takes ownership of raw ptr connection
		 *
		 * @param connection
		 */
		MySQLConnection(sql::Connection* connection);
        /**
		 * Takes ownership of scope ptr connection
		 *
		 * @param connection
		 */
        MySQLConnection(Scope<sql::Connection>&& connection);
		/**
		 * Create connection object and connects to database using ConnectOptionsMap
		 *
		 * @param driver MySQL driver instance
		 * @param conn_options is a map with properties: https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-connect-options.html
		 */
		MySQLConnection(sql::Driver* driver, sql::ConnectOptionsMap const& conn_options);
		/**
		 * Create connection object and connects to database
		 *
		 * @param driver MySQL driver instance
		 * @param conn_options is a map with properties: https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-connect-options.html
		 */
		MySQLConnection(sql::Driver* driver, std::string_view host, std::string_view user, std::string_view password);

        /**
		 * Connects to existing database using options map
		 *
		 * @param conn_options is a map with properties: https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-connect-options.html
		 * @param ec callback error
		 *
		 * @return true if connection success
		 */
		bool Connect(sql::ConnectOptionsMap const& conn_options);
		/**
		 * Connects to existing database using options map.
		 *
		 * @param conn_options is a map with properties: https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-connect-options.html
		 * @param ec callback error
		 *
		 * @return true if connection success
		 */
		bool Connect(sql::ConnectOptionsMap const& conn_options, Exception& ec);

		/**
		 * Connects to database using options map
		 *
		 * @param conn_options is a map with properties: https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-connect-options.html
		 *
		 * @return true if connection success
		 */
		std::future<bool> ConnectAsync(sql::ConnectOptionsMap const& conn_options);
		/**
		 * Connects to existing database async
		 *
		 * @param host
		 * @param user
		 * @param password
		 * 
		 * @param ec callback error
		 *
		 * @return true if connection success
		 */
		std::future<bool> ConnectAsync(std::string_view host, std::string_view user, std::string_view password);

    private:


    };
}