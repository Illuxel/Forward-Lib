#pragma once

#include "fl/utils/Memory.hpp"
#include "fl/utils/Exception.hpp"
#include "fl/utils/DateTime.hpp"

#include <jdbc/cppconn/resultset.h>

namespace Forward::DBTypes {
	
	template<class Type>
	static constexpr Type GetResultFromType(sql::ResultSet* result, uint32_t column)
	{
		if (!result)
			throw std::runtime_error("Result is not initialized");

		// Blob type
		if constexpr (std::is_same<Type, std::istream*>::value) {
			return result->getBlob(column);
		}
		// Text
		else if constexpr (std::is_same<Type, char const*>::value) {
			return result->getString(column);
		}
		else if constexpr (std::is_same<Type, std::string>::value) {
			return result->getString(column);
		}
		else if constexpr (std::is_same<Type, std::string_view>::value) {
			return result->getString(column).c_str();
		}
		// Integers and boolean
		else if constexpr (std::is_same<Type, bool>::value) {
			return result->getBoolean(column);
		}
		else if constexpr (std::is_same<Type, double>::value) {
			return result->getDouble(column);
		}
		else if constexpr (std::is_same<Type, int32_t>::value) {
			return result->getInt(column);
		}
		else if constexpr (std::is_same<Type, uint32_t>::value) {
			return result->getUInt(column);
		}
		else if constexpr (std::is_same<Type, int64_t>::value) {
			return result->getInt64(column);
		}
		else if constexpr (std::is_same<Type, uint64_t>::value) {
			return result->getUInt64(column);
		}
		else {
			throw std::runtime_error("Unsupported type");
		}
	}

	class Result
	{
	private:
		Scope<sql::ResultSet> result_ = nullptr;

	public:
		Result();
		Result(sql::ResultSet* result);
		Result(Scope<sql::ResultSet>&& right);

		Result(Result&& result) noexcept;
		Result& operator=(Result&& right) noexcept;

		template<class Type>
		constexpr Type Get(uint32_t column) const
		{
			if (column == 0)
				throw std::invalid_argument("Invalid column name");

			if (IsEmpty())
				throw std::runtime_error("Query result is empty");
			
			return GetResultFromType<Type>(result_.get(), column);
		}

		template<class Type>
		constexpr Type Get(std::string_view column) const
		{
			int column = result_->findColumn(column.data());
			return Get<Type>(column);
		}

		uint32_t RowCount() const;
		uint32_t ColumnCount() const;

		bool IsValid() const;
		bool IsEmpty() const;

		explicit operator bool() const
		{
			return IsEmpty();
		}

		Result(Result const&) = delete;
		Result& operator=(Result const&) = delete;
	};
}