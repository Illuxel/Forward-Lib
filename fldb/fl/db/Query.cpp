#include "fl/db/Query.hpp"

namespace Forward::DBTypes {

	Query::Query()
		: query_data_(nullptr)
	{
	}
	Query::Query(sql::Statement* statement)
		: query_data_(statement)
	{
	}
	Query::Query(Scope<sql::Statement>&& statement)
		: query_data_(nullptr)
	{
		query_data_ = std::move(statement);
	}
	Query::~Query()
	{
	}

	void Query::SetQuery()
	{
	}

	Exception Query::GetException() const
	{
		return query_ec_;
	}

	Result Query::GetResult() const
	{
		return Result();
	}

	void Query::Execute()
	{
	}

	Result Query::Execute(std::string_view sql)
	{
		Result result;

		if (!IsValid())
		{
			return result;
		}

		try
		{
			query_data_->executeQuery(sql.data());
		}
		catch (std::exception const& e)
		{
			query_ec_ = e;
		}

		return result;
	}

	bool Query::IsValid() const
	{
		return (bool)query_data_;
	}
	bool Query::IsExecuted() const
	{
		return query_ec_.IsError();
	}

}