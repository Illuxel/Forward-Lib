#pragma once

#include "fl/db/private/mysql/DriverLock.hpp"

#include "fl/db/Query.hpp"
#include "fl/db/PreparedQuery.hpp"

#include "fl/db/Result.hpp"

#include "fl/utils/Log.hpp"

#include <future>

namespace Forward {

	/**
	 * Represents database connection
	 */
	class DBConnection
	{
	private:
		sql::Driver* driver_;
		Scope<sql::Connection> connection_;

		bool is_scheme;

		mutable std::mutex conn_mtx_;           // for connection ptr ONLY
		mutable std::shared_mutex data_mtx_;    // for everything else

	public:
		/**
		 * @param driver MySQL driver instance
		 */
		DBConnection(sql::Driver* driver);
		/**
		 * Takes ownership of raw ptr connection
		 *
		 * @param connection
		 */
		DBConnection(sql::Connection* connection);
		/**
		 * Takes ownership of scope ptr connection
		 *
		 * @param connection
		 */
		DBConnection(Scope<sql::Connection>&& connection);
		/**
		 * Create connection object and connects to database using ConnectOptionsMap
		 *
		 * @param driver MySQL driver instance
		 * @param conn_options is a map with properties: https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-connect-options.html
		 */
		DBConnection(sql::Driver* driver, sql::ConnectOptionsMap const& conn_options);
		/**
		 * Create connection object and connects to database
		 *
		 * @param driver MySQL driver instance
		 * @param conn_options is a map with properties: https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-connect-options.html
		 */
		DBConnection(sql::Driver* driver, std::string_view host, std::string_view user, std::string_view password);

		virtual ~DBConnection();

		/**
		 * Sets current database scheme/name
		 *
		 * @param scheme database scheme/name
		 */
		void SetActiveSchema(std::string_view scheme);

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
		 * Connects to existing database. Handles exceptions in method scope
		 *
		 * @param host
		 * @param user
		 * @param password
		 *
		 * @return true if connection success
		 */
		bool Connect(std::string_view host, std::string_view user, std::string_view password);
		/**
		 * Connects to database
		 *
		 * @param host
		 * @param user
		 * @param password
		 * 
		 * @param ec callback error
		 *
		 * @return true if connection success
		 */
		bool Connect(std::string_view host, std::string_view user, std::string_view password, Exception& ec);

		/**
		 * Connects to database using options map
		 *
		 * @param conn_options is a map with properties: https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-connect-options.html
		 *
		 * @return true if connection success
		 */
		std::future<bool> AsyncConnect(sql::ConnectOptionsMap const& conn_options);
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
		std::future<bool> AsyncConnect(std::string_view host, std::string_view user, std::string_view password);

		/**
		 * Executes query. Handles exceptions in method scope
		 *
		 * @param query SQL query
		 *
		 * @return query result or nullptr
		 */
		virtual DBTypes::Result Execute(std::string_view query);
		/**
		 * Executes query
		 *
		 * @param query SQL query
		 * @param ec callback error
		 *
		 * @return query result or nullptr
		 */
		virtual DBTypes::Result Execute(std::string_view query, Exception& ec);
		/**
		 * Executes query async
		 *
		 * @param query SQL query
		 *
		 * @return future object of result
		 */
		virtual std::future<DBTypes::Result> AsyncExecute(std::string_view query);

		/**
		 * Executes query with arguments. Handles exceptions in method scope
		 *
		 * @param query    SQL query
		 * @param args     argument list for query parameters.
		 *                  Each argument represents '?' in SQL query
		 *
		 * @return query result or nullptr
		 */
		template<typename ...Args>
		DBTypes::Result Execute(std::string_view query, Args&&... args)
		{
			Exception ec;
			DBTypes::Result result = Execute(query, ec, std::forward<Args>(args)...);

			return result;
		}
		/**
		 * Executes query with arguments.
		 *
		 * @param query    SQL query
		 * @param args     argument list for query parameters.
		 *                  Each argument represents '?' in SQL query
		 * @param ec callback error
		 *
		 * @return query result or nullptr
		 */
		template<typename ...Args>
		DBTypes::Result Execute(std::string_view query, Exception& ec, Args&&... args)
		{
			if (!IsConnected())
			{
				FL_LOG("DBConnection", "is not connected");
				return nullptr;
			}
			if (!IsActiveSchema())
			{
				FL_LOG("DBConnection", "scheme was not set");
				return nullptr;
			}

			DBTypes::Result result;
			MySQL::DriverLock d_lock(driver_);

			{
				std::lock_guard lock(conn_mtx_);

				try
				{
					DBTypes::PreparedQuery query(connection_->prepareStatement(query.data()));

					//BindValue(query.get(), std::forward<Args>(args)...);

					result = std::move(query.Execute());
				}
				catch (std::exception const& e)
				{
					ec = e;
				}
			}

			return result;
		}
		/**
		 * Executes query with arguments in separate thread
		 * 
		 * @return future object of query result
		 */
		template<typename ...Args>
		std::future<DBTypes::Result> AsyncExecute(std::string_view query, Args&&... args)
		{
			std::future<DBTypes::Result> future = std::async(
				std::launch::async,
				[&]()
				{
					DBTypes::Result result = Execute(query, std::forward<Args>(args)...);

					return result;
				});

			return future;
		}

		/**
		 * Closes database connection
		 */
		void Close();

		/**
		 * Checks if database has  connection
		 *
		 * @return true if connection ptr is initialized, otherwise will return false
		 */
		bool IsValid() const;
		/**
		 * Checks if database has established connection
		 *
		 * @return true if connection exist, otherwise false
		 */
		bool IsConnected() const;
		/**
		 *	Checks if database scheme/name is set
		 *
		 *  @return true if scheme is set, otherwise false
		 */
		bool IsActiveSchema() const;
	};
} // namespace Forward
