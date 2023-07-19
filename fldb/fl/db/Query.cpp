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

	Result Query::Execute(std::string_view sql)
	{
		return Result();
	}

	bool Query::IsValid() const
	{
		return false;
	}
}