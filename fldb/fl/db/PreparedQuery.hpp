#pragma once

#include "fl/db/Query.hpp"

#include <jdbc/cppconn/prepared_statement.h>

namespace Forward::DBTypes {

	template<class Type>
	static constexpr void SetQueryArg(sql::PreparedStatement* query, uint32_t column, Type value)
	{
		if (!query)
			throw std::runtime_error("Query is not initialized");

		if constexpr (std::is_same<Type, std::istream*>) {

			if constexpr (std::is_pointer<Type>) {
				query->setBlob(column, value);
			}
		}
		else if constexpr (std::is_same<Type, std::string>) {
			query->setString(column, value);
		}
		else if constexpr (std::is_same<Type, char const*>) {
			query->setString(column, value);
		}
		else if constexpr (std::is_same<Type, std::string_view) {
			query->setString(column, value.data());
		}
		else if constexpr (std::is_same<Type, DateTime>) {
			query->setString(column, value);
		}
		else if constexpr (std::is_same<Type, bool>) {
			query->setBoolean(column, value);
		}
		else if constexpr (std::is_same<Type, double>) {
			query->setDouble(column, value);
		}
		else if constexpr (std::is_same<Type, int32_t>) {
			query->setInt(column, value);
		}
		else if constexpr (std::is_same<Type, uint32_t>) {
			query->setUInt(column, value);
		}
		else if constexpr (std::is_same<Type, int64_t>) {
			query->setInt64(column, value);
		}
		else if constexpr (std::is_same<Type, uint64_t>) {
			query->setUInt64(column, value);
		}
		else {
			throw std::runtime_error("Unsupported type");
		}
	}

	class PreparedQuery : public Query
	{
	private:
		uint32_t bind_val_;

	public:
		PreparedQuery();
		PreparedQuery(sql::PreparedStatement* statement);
		PreparedQuery(Scope<sql::PreparedStatement>&& statement);

		virtual ~PreparedQuery() override;

		template <class ...Args>
		void BindValue(Args&& ...args)
		{

		}
		template <class Type>
		void BindValue(uint32_t index, Type value)
		{
			
		}

	};
}