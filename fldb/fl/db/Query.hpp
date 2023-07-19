#pragma once

#include "fl/db/Result.hpp"

#include <jdbc/cppconn/statement.h>

namespace Forward::DBTypes {

	class Query
	{
	protected:
		Scope<sql::Statement> query_data_;

	public:
		Query();
		Query(sql::Statement* statement);
		Query(Scope<sql::Statement>&& statement);

		virtual ~Query();

		virtual Result Execute(std::string_view sql = "");

		virtual bool IsValid() const;
	};
}