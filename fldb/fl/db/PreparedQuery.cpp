#include "fl/db/PreparedQuery.hpp"

namespace Forward::DBTypes {

	PreparedQuery::PreparedQuery()
		: Query()
	{

	}
	PreparedQuery::PreparedQuery(sql::PreparedStatement* statement)
		: Query(statement)
	{

	}
	PreparedQuery::PreparedQuery(Scope<sql::PreparedStatement>&& statement)
		: Query(std::move(statement))
	{

	}

	PreparedQuery::~PreparedQuery() {}

}