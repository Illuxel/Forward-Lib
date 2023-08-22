#include "fl/db/Result.hpp"

namespace Forward::DBTypes {

	Result::Result() {}

	Result::Result(sql::ResultSet* result) 
		: result_(result) {}

	Result::Result(Scope<sql::ResultSet>&& result)
	{
		result_ = std::move(result);
	}

	Result::Result(Result&& right) noexcept
	{
		*this = std::move(right);
	}

	uint32_t Result::RowCount() const
	{
		return 0;
	}
	uint32_t Result::ColumnCount() const
	{
		return 0;
	}

	bool Result::IsValid() const
	{
		return (bool)result_;
	}
	bool Result::IsEmpty() const
	{
		return !IsValid() || !result_->first();
	}

	Result& Result::operator=(Result&& right) noexcept
	{
		result_ = std::move(right.result_);
		return *this;
	}
}