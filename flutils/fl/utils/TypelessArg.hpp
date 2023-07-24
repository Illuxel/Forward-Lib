#pragma once

#include <string>

#include <optional>

namespace Forward {
	
	template<typename ValueType>
	class TypelessArg
	{
	private:
		std::string arg_name_;
		std::string arg_format_;

		std::optional<ValueType> arg_value_;

	public:
		constexpr TypelessArg() {}

		constexpr ValueType const& GetData() const&
		{
			return arg_value_.value();
		}
		constexpr ValueType& GetData() &
		{
			return arg_value_.value();
		}
		constexpr ValueType&& GetData() &&
		{
			return arg_value_.value();
		}
		constexpr ValueType&& GetData() const&&
		{
			return arg_value_.value();
		}



		constexpr bool HasValue() const
		{
		
		}

		constexpr operator ValueType const&() const&
		{
			return arg_value_.value();
		}
		constexpr operator ValueType&()&
		{
			return arg_value_.value();
		}
		constexpr operator ValueType &&()&&
		{
			return arg_value_.value();
		}
		constexpr operator ValueType &&() const&&
		{
			return arg_value_.value();
		}
	};
}
