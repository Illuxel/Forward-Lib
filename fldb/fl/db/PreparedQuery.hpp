#pragma once

#include "fl/db/Query.hpp"

namespace Forward::Database {

	template<typename Connector>
	class PreparedQuery
	{
	private:
		
		
	public:
		PreparedQuery(DBConnection<Connector> const& ) {}
		~PreparedQuery() {}

		/**
		 * Executes query with arguments. Handles exceptions in method scope
		 *
		 * @param sql    SQL query
		 * @param args   argument list for query parameters.
		 *               Each argument represents '?' in SQL query
		 * @return query result or nullptr
		 */
		template<typename ...Args>
		Result<Connector> Execute(std::string_view sql, Args&&... args)
		{
			Exception ex;
			Result<Connector> result = Execute(sql, ex, std::forward<Args>(args)...);

			return result;
		}
		/**
		 * Executes query with arguments.
		 *
		 * @param sql	SQL query
		 * @param args	argument list for query parameters.
		 *              Each argument represents '?' in SQL query
		 * @param ex 	callback error
		 * @return 		query result or nullptr
		 */
		template<typename ...Args>
		Result<Connector> Execute(std::string_view sql, Exception& ex, Args&&... args)
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

			Result<Connector> result;

			{
				std::lock_guard lock(mtx_);

				try
				{
					PreparedQuery query(conn_->prepareStatement(sql.data()));

					query.BindValue(std::forward<Args...>(args)...);
					query.Execute();

					//result = std::move(query.Execute());
				}
				catch (Exception const& e)
				{
					ex = e;
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
		std::future<Result<Connector>> ExecuteAsync(std::string_view sql, Args&&... args)
		{
			std::future<Result<Connector>> future = 
			std::async(std::launch::async,
			[&]()
			{
				Result<Connector> result = Execute(sql, std::forward<Args>(args)...);
				return result;
			});

			return future;
		}
		/**
		 * Executes query with arguments in separate thread
		 * 
		 * @return future object of query result
		 */
		template<typename ...Args>
		std::future<Result<Connector>> ExecuteAsync(std::string_view sql, Exception& ex, Args&&... args)
		{
			std::future<Result<Connector>> future = 
			std::async(std::launch::async,
			[&]()
			{
				Result<Connector> result = Execute(sql, ex, std::forward<Args>(args)...);
				return result;
			});

			return future;
		}

		template <typename ...Args>
		constexpr void BindValue(Args&& ...args)
		{
			//(SetQueryArg(std::forward(args)), ...);
		}

		template <typename Type>
		constexpr void BindValue(uint32_t index, Type value)
		{
			
		}
	};
} // namespace Forward::Database