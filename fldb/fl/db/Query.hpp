#pragma once

#include "fl/db/Result.hpp"

namespace Forward::DBTypes {

	class Query
	{
	protected:
		Exception query_ec_;
		Scope<sql::Statement> query_data_ = nullptr;

	public:
		Query();
		Query(sql::Statement* statement);
		Query(Scope<sql::Statement>&& statement);

		virtual ~Query();

		void SetQuery();

		Exception GetException() const;
		Result GetResult() const;

		virtual void Execute();
		virtual Result Execute(std::string_view sql);

		virtual bool IsValid() const;
		virtual bool IsExecuted() const;
	};
}