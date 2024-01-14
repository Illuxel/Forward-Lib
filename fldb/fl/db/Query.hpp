#pragma once

#include "fl/db/Result.hpp"

namespace Forward::Database {

	template<typename Connector>
	class Query
	{
	protected:
		Exception ex_;
		DBConnection<Connector> conn_;

	public:
		Query() {}
		~Query() {}

		void SetQuery()	{}

		Exception GetError() const
		{
			return ex_;
		}

		/**
		 * Executes query. Handles exceptions in method scope
		 *
		 * @param sql SQL query
		 * @return query result
		 */
		Result<Connector> Execute(std::string_view sql) 
		{
			Exception ex;
			auto result = Execute(sql, ex);

			return result;
		}
		/**
		 * Executes query
		 *
		 * @param sql SQL query
		 * @param ex callback error
		 * @return query result
		 */
		Result<Connector> Execute(std::string_view sql, Exception& ex) {}

		/**
		 * Executes query async
		 *
		 * @param sql SQL query
		 * @return future object of result
		 */
		std::future<Result<Connector>> ExecuteAsync(std::string_view sql, Exception& ex) = 0;

		bool IsValid() const
		{
			return (bool)query_data_;
		}
		bool IsExecuted() const
		{
			return ex_.IsError();
		}
	};
} // namespace Forward::Database