#pragma once

#include <string>

#include <optional>

namespace Forward {
	
	template<typename ValueType>
	class TypedArg
	{
	protected:
		std::string arg_name_;
		std::string arg_format_;

		std::optional<ValueType> arg_value_;

	public:
		constexpr TypedArg()
			: arg_value_(std::nullopt) 
		{
		
		}
		constexpr TypedArg(std::string_view arg_name, ValueType const& arg_value, std::string_view arg_format = "")
			: arg_name_(arg_name)
			, arg_format_(arg_format)
			, arg_value_(arg_value_)
		{
			
		}



		constexpr ValueType const& GetData() const&
		{
			return arg_value_.value_or(ValueType());
		}
		constexpr ValueType& GetData() &
		{
			return arg_value_.value_or(ValueType());
		}
		constexpr ValueType&& GetData() &&
		{
			return arg_value_.value_or(ValueType());
		}
		constexpr ValueType&& GetData() const&&
		{
			return arg_value_.value_or(ValueType());
		}

		constexpr bool IsValid() const 
		{
			return !HasName() && HasValue();
		}

		bool HasName() const
		{
			return !arg_name_.empty();
		}
		bool HasFormat() const
		{
			return !arg_format_.empty();
		}

		constexpr bool HasValue() const
		{
			return arg_value_.has_value();
		}

		bool operator==(std::string_view right) const
		{
			return HasName() && arg_name_ == right;
		}
		bool operator==(TypedArg<ValueType> const& right) const
		{
			return arg_name_ == right.arg_name_;
		}

		constexpr TypedArg(TypedArg<ValueType>&& right) noexcept
		{
			*this = std::move(right);
		}
		constexpr TypedArg(TypedArg<ValueType> const& right) noexcept
		{
			*this = right;
		}

		constexpr TypedArg<ValueType>& operator=(TypedArg<ValueType>&& right) noexcept
		{
			arg_name_ = std::move(right.arg_name_);
			arg_format_ = std::move(right.arg_format_);
			arg_value_ = std::move(right.arg_value_);

			return *this;
		}
		constexpr TypedArg<ValueType>& operator=(TypedArg<ValueType> const& right) noexcept
		{
			arg_name_ = right.arg_name_;
			arg_format_ = right.arg_format_;
			arg_value_ = right.arg_value_;

			return *this;
		}

		constexpr operator std::string const&() const&
		{
			return arg_name_;
		}
	};
}
