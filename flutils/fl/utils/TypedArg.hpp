#pragma once

#include <string>
#include <vector>

#include <optional>

namespace Forward {
	
	/**
	 * 
	 * 
	 */
	template<class ValueType>
	class TypedArg
	{
	private:
		std::string arg_name_;
		std::string arg_format_;

		std::optional<ValueType> arg_value_;

	public:
		constexpr TypedArg()
			: arg_value_(std::nullopt) {}

		constexpr TypedArg(std::string_view arg_name)
			: arg_name_(arg_name) {}

		constexpr TypedArg(std::string_view arg_name, ValueType const& arg_value, 
			std::string_view arg_format = "")
			: arg_name_(arg_name)
			, arg_format_(arg_format)
			, arg_value_(arg_value_) {}

		void SetName(std::string_view arg_name)
		{
			arg_name_ = arg_name;
		}
		void SetFormat(std::string_view arg_format)
		{
			arg_format_ = arg_format;
		}

		/**
		 * This method returns argument name
		 */
		std::string GetName() const
		{
			return arg_name_;
		}
		/**
		 * This method returns argument parsing format
		 */
		std::string GetFormat() const
		{
			return arg_format_;
		}

		std::string GetParsed() const 
		{
			return "";
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

		/**
		 * This method returns size of argument name
		 */
		uint64_t Size() const
		{
			return GetName().size();
		}

		constexpr bool IsValid() const 
		{
			return !HasName() && HasValue();
		}

		/**
		 * This method checks if argument name is empty or not
		 */
		bool HasName() const
		{
			return !arg_name_.empty();
		}
		/**
		 * This method checks if parsing format was specified
		 */
		bool HasFormat() const
		{
			return !arg_format_.empty();
		}

		constexpr bool HasValue() const
		{
			return arg_value_.has_value();
		}

		static std::vector<TypedArg<ValueType>>
		MakeArgs(std::vector<std::string> const& args, std::string_view arg_format)
		{
			std::vector<TypedArg<ValueType>> arg_list;

			for (auto const& arg_name : args)
			{
				arg_list.emplace_back(arg_name, arg_format);
			}

			return arg_list;
		}

		static std::vector<TypedArg<ValueType>> 
		MakeArgs(std::initializer_list<std::pair<std::string, ValueType>> args, std::string_view arg_format = "")
		{
			std::vector<TypedArg<ValueType>> arg_list;

			for (auto const& pair : args)
			{
				arg_list.emplace_back(pair.first, pair.second, arg_format);
			}

			return arg_list;
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
