#include "fl/db/Result.hpp"

namespace Forward::DBTypes {

	Result::Result() 
		: result_(nullptr)
	{
	}

	Result::Result(Result&& right)
		: result_(nullptr)
	{
		*this = std::move(right);
	}

	Result::Result(sql::ResultSet* result) 
		: result_(result)
	{
	}
	Result::Result(Scope<sql::ResultSet>&& result) 
		: result_(nullptr)
	{
		result_ = std::move(result);
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