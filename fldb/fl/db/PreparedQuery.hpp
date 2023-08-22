<<<<<<< Updated upstream
#pragma once

#include "fl/db/Query.hpp"

namespace Forward::DBTypes {

	template<class Type>
	static constexpr void SetQueryArg(sql::PreparedStatement* query, uint32_t index, Type value)
	{
		if (!query)
			throw std::runtime_error("Query is not initialized");

		if constexpr (std::is_same<Type, std::istream*>()) {

			if constexpr (std::is_pointer<Type>()) {
				query->setBlob(index, value);
			}
		}
		else if constexpr (std::is_same<Type, std::string>()) {
			query->setString(index, value);
		}
		else if constexpr (std::is_same<Type, char const*>()) {
			query->setString(index, value);
		}
		else if constexpr (std::is_same<Type, std::string_view>()) {
			query->setString(index, value.data());
		}
		else if constexpr (std::is_same<Type, DateTime>()) {
			query->setString(index, value.ToString());
		}
		else if constexpr (std::is_same<Type, bool>()) {
			query->setBoolean(index, value);
		}
		else if constexpr (std::is_same<Type, double>()) {
			query->setDouble(index, value);
		}
		else if constexpr (std::is_same<Type, int32_t>()) {
			query->setInt(index, value);
		}
		else if constexpr (std::is_same<Type, uint32_t>()) {
			query->setUInt(index, value);
		}
		else if constexpr (std::is_same<Type, int64_t>()) {
			query->setInt64(index, value);
		}
		else if constexpr (std::is_same<Type, uint64_t>()) {
			query->setUInt64(index, value);
		}
		else {
			throw std::runtime_error("Unsupported type");
		}
	}

	class PreparedQuery : public Query
	{
	public:
		PreparedQuery();
		PreparedQuery(sql::PreparedStatement* statement);
		PreparedQuery(Scope<sql::PreparedStatement>&& statement);

		virtual ~PreparedQuery() override;

		template <class ...Args>
		constexpr void BindValue(Args&& ...args)
		{

			

			//(SetQueryArg(std::forward(args)), ...);
		}

		template <class Type>
		constexpr void BindValue(uint32_t index, Type value)
		{
			
		}
	};
=======
#pragma once

#include "fl/db/Query.hpp"

namespace Forward::DBTypes {

	template<class Type>
	static constexpr void SetQueryArg(sql::PreparedStatement* query, uint32_t index, Type value)
	{
		if (!query)
			throw std::runtime_error("Query is not initialized");

		if constexpr (std::is_same<Type, std::istream*>()) {

			if constexpr (std::is_pointer<Type>()) {
				query->setBlob(index, value);
			}
		}
		else if constexpr (std::is_same<Type, std::string>()) {
			query->setString(index, value);
		}
		else if constexpr (std::is_same<Type, char const*>()) {
			query->setString(index, value);
		}
		else if constexpr (std::is_same<Type, std::string_view>()) {
			query->setString(index, value.data());
		}
		else if constexpr (std::is_same<Type, DateTime>()) {
			query->setString(index, value.ToString());
		}
		else if constexpr (std::is_same<Type, bool>()) {
			query->setBoolean(index, value);
		}
		else if constexpr (std::is_same<Type, double>()) {
			query->setDouble(index, value);
		}
		else if constexpr (std::is_same<Type, int32_t>()) {
			query->setInt(index, value);
		}
		else if constexpr (std::is_same<Type, uint32_t>()) {
			query->setUInt(index, value);
		}
		else if constexpr (std::is_same<Type, int64_t>()) {
			query->setInt64(index, value);
		}
		else if constexpr (std::is_same<Type, uint64_t>()) {
			query->setUInt64(index, value);
		}
		else {
			throw std::runtime_error("Unsupported type");
		}
	}

	class PreparedQuery : public Query
	{
	public:
		PreparedQuery();
		PreparedQuery(sql::PreparedStatement* statement);
		PreparedQuery(Scope<sql::PreparedStatement>&& statement);

		virtual ~PreparedQuery() override;

		template <class ...Args>
		constexpr void BindValue(Args&& ...args)
		{

			

			//(SetQueryArg(std::forward(args)), ...);
		}

		template <class Type>
		constexpr void BindValue(uint32_t index, Type value)
		{
			
		}
	};
>>>>>>> Stashed changes
}