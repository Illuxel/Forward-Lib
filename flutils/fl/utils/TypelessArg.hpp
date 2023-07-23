#pragma once

#include <map>
#include <string>
#include <vector>

#include <optional>

namespace Forward {
	
	template<typename Type>
	class TypelessArg
	{
	private:
		std::string arg_name_;
		std::string arg_format_;

		std::optional<Type> arg_data_;

	public:
		TypelessArg() {}

		Type GetData()&&
		{
			
		}
		Type GetData() const&
		{
			
		}

		bool IsValid() const;
	};
}
