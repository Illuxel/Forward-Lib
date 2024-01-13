#pragma once

#include <string>
#include <string_view>
#include <vector>

#include <optional>

namespace Forward {
	
	/**
	 * Key value pair class that supports argument parsing
	 * 
	 */
	template<class ValueType>
	class TypedArg
	{
	private:
		std::string arg_parsed_;
		std::string key_, format_;

		std::optional<ValueType> value_ = std::nullopt;

	public:
		constexpr TypedArg() {}

		constexpr TypedArg(std::string_view key)
			: key_(key) {}

		constexpr TypedArg(
			std::string_view key, 
			ValueType const& value, 
			std::string_view format = "")
			: key_(key)
			, format_(format)
			, value_(value)
		{
		}

		constexpr TypedArg(
			std::string_view key,
			ValueType&& arg_value,
			std::string_view format = "")
			: key_(key)
			, format_(format)
			, value_(std::move(arg_value))
		{
		}

		void SetName(std::string_view key)
		{
			key_ = key;
		}
		void SetFormat(std::string_view format)
		{
			format_ = format;
		}

		/**
		 * This method returns argument name
		 */
		std::string GetName() const
		{
			return key_;
		}
		/**
		 * This method returns argument parsing format
		 */
		std::string GetFormat() const
		{
			return format_;
		}
		/**
		 * 
		 */
		std::string GetParsed() const 
		{
			return "";
		}

		constexpr ValueType& GetData() &
		{
			return value_.value();
		}
		constexpr ValueType const& GetData() const&
		{
			return value_.value();
		}
		constexpr ValueType&& GetData() &&
		{
			return std::move(value_.value());
		}
		constexpr ValueType const&& GetData() const&&
		{
			return std::move(value_.value());
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
			return !key_.empty();
		}
		/**
		 * This method checks if parsing format was specified
		 */
		bool HasFormat() const
		{
			return !format_.empty();
		}

		constexpr bool HasValue() const
		{
			return value_.has_value();
		}

		static std::vector<TypedArg<ValueType>>
		MakeArgs(std::vector<std::string> const& args, std::string_view format = "")
		{
			std::vector<TypedArg<ValueType>> arg_list;

			for (auto const& key : args)
			{
				arg_list.emplace_back(key, format);
			}

			return arg_list;
		}

		static std::vector<TypedArg<ValueType>> 
		MakeArgs(std::initializer_list<std::pair<std::string, ValueType>> args, std::string_view format = "")
		{
			std::vector<TypedArg<ValueType>> arg_list;

			for (auto const& [key, value] : args)
			{
				arg_list.emplace_back(key, value, format);
			}

			return arg_list;
		}

		bool operator==(std::string_view right) const
		{
			if (!HasFormat())
				return key_ == right;

			return GetParsed() == right;
		}
		bool operator==(TypedArg<ValueType> const& right) const
		{
			return GetName() == right.GetName()
				&& GetFormat() == right.GetFormat();
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
			key_ = std::move(right.key_);
			format_ = std::move(right.format_);
			value_ = std::move(right.value_);

			return *this;
		}
		constexpr TypedArg<ValueType>& operator=(TypedArg<ValueType> const& right) noexcept
		{
			key_ = right.key_;
			format_ = right.format_;
			value_ = right.value_;

			return *this;
		}

		constexpr operator std::string const&() const&
		{
			return key_;
		}
	};
}
