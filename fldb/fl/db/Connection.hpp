#pragma once

#include "fl/db/Query.hpp"
#include "fl/db/PreparedQuery.hpp"

#include "fl/db/Result.hpp"

namespace Forward::Database {

	/**
	 * Represents database connection
	 */
	template<class SQLConnector>
	class Connection
	{
	private:
		mutable std::shared_mutex data_mtx_;    // for everything else

	public:
		virtual ~Connection();

		/**
		 * Sets current database scheme/name
		 *
		 * @param scheme database scheme/name
		 */
		void SetActiveSchema(std::string_view scheme);

		/**
		 * Executes query. Handles exceptions in method scope
		 *
		 * @param sql SQL query
		 *
		 * @return query result or nullptr
		 */
		virtual Result<SQLConnector> Execute(std::string_view sql);
		/**
		 * Executes query
		 *
		 * @param sql SQL query
		 * @param ec callback error
		 *
		 * @return query result or nullptr
		 */
		virtual Result<SQLConnector> Execute(std::string_view sql, Exception& ec);
		/**
		 * Executes query async
		 *
		 * @param sql SQL query
		 *
		 * @return future object of result
		 */
		virtual std::future<Result> AsyncExecute(std::string_view sql);

		/**
		 * Executes query with arguments. Handles exceptions in method scope
		 *
		 * @param sql    SQL query
		 * @param args     argument list for query parameters.
		 *                  Each argument represents '?' in SQL query
		 *
		 * @return query result or nullptr
		 */
		template<typename ...Args>
		Result Execute(std::string_view sql, Args&&... args)
		{
			Exception ec;
			Result result = Execute(sql, ec, std::forward<Args>(args)...);

			return result;
		}
		/**
		 * Executes query with arguments.
		 *
		 * @param sql    SQL query
		 * @param args     argument list for query parameters.
		 *                  Each argument represents '?' in SQL query
		 * @param ec callback error
		 *
		 * @return query result or nullptr
		 */
		template<typename ...Args>
		Result Execute(std::string_view sql, Exception& ec, Args&&... args)
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

			Result result;
			//MySQL::DriverLock d_lock(driver_);
			{
				std::lock_guard lock(conn_mtx_);

				try
				{
					PreparedQuery query(connection_->prepareStatement(sql.data()));

					query.BindValue(std::forward<Args>(args)...);
					query.Execute();

					//result = std::move(query.Execute());
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
		std::future<Result> AsyncExecute(std::string_view sql, Args&&... args)
		{
			std::future<Result> future = std::async(
				std::launch::async,
				[&]()
				{
					Result result = Execute(sql, std::forward<Args>(args)...);
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
