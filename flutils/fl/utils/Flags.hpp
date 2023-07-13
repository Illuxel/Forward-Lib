#pragma once

#include <type_traits>

namespace Forward {

    template <typename Enum>
    class EnumFlags
    {
    private:
        static_assert(std::is_enum_v<Enum>, "EnumFlags requires an enum type.");

        using underlying_type = std::underlying_type_t<Enum>;

        underlying_type enum_val_;

    public:
        constexpr EnumFlags() noexcept
            : enum_val_{} {}
        constexpr EnumFlags(Enum value) noexcept
            : enum_val_{static_cast<underlying_type>(value)} {}

        constexpr EnumFlags(EnumFlags const& value) noexcept
            : enum_val_{value.enum_val_} {}

        constexpr bool Test(EnumFlags other) const 
        {
            return (enum_val_ & static_cast<underlying_type>(other)) != 0;
        }

        constexpr bool operator!() const noexcept
        {
            return enum_val_ == 0;
        }
        constexpr bool operator==(EnumFlags other) const noexcept
        {
            return enum_val_ == other.enum_val_;
        }
        constexpr bool operator!=(EnumFlags other) const noexcept
        {
            return enum_val_ != other.enum_val_;
        }

        constexpr EnumFlags& operator|=(EnumFlags other) noexcept
        {
            enum_val_ |= other.enum_val_;
            return *this;
        }
        constexpr EnumFlags& operator&=(EnumFlags other) noexcept
        {
            enum_val_ &= other.enum_val_;
            return *this;
        }
        constexpr EnumFlags& operator^=(EnumFlags other) noexcept
        {
            enum_val_ ^= other.enum_val_;
            return *this;
        }
        
        constexpr EnumFlags operator|(EnumFlags other) const noexcept
        {
            return EnumFlags{*this} |= other;
        }
        constexpr EnumFlags operator&(EnumFlags other) const noexcept
        {
            return EnumFlags{*this} &= other;
        }
        constexpr EnumFlags operator^(EnumFlags other) const noexcept
        {
            return EnumFlags{*this} ^= other;
        }
        constexpr EnumFlags operator~() const noexcept
        {
            return EnumFlags{static_cast<Enum>(~enum_val_)};
        }

        constexpr explicit operator bool() const noexcept
        {
            return enum_val_ != 0;
        }
        constexpr explicit operator underlying_type() const noexcept
        {
            return enum_val_;
        }
    };
}

#define ENABLE_ENUM_FLAGS(Enum, FlagsName) \
    using FlagsName = Forward::EnumFlags<Enum>

#define DECLARE_ENUM_OPERATORS(Enum) \
    static_assert(std::is_enum_v<Enum>, "EnumFlags requires an enum type."); \
    inline constexpr Enum operator|(Enum lhs, Enum rhs) noexcept \
    {                                                            \
        return static_cast<Enum>(                                \
            static_cast<std::underlying_type_t<Enum>>(lhs) |     \
            static_cast<std::underlying_type_t<Enum>>(rhs));     \
    }                                                            \
    inline constexpr Enum operator&(Enum lhs, Enum rhs) noexcept \
    {                                                            \
        return static_cast<Enum>(                                \
            static_cast<std::underlying_type_t<Enum>>(lhs) &     \
            static_cast<std::underlying_type_t<Enum>>(rhs));     \
    }                                                            \
    inline constexpr Enum operator^(Enum lhs, Enum rhs) noexcept \
    {                                                            \
        return static_cast<Enum>(                                \
            static_cast<std::underlying_type_t<Enum>>(lhs) ^     \
            static_cast<std::underlying_type_t<Enum>>(rhs));     \
    }                                                            \
    inline constexpr Enum operator~(Enum value) noexcept         \
    {                                                            \
        return static_cast<Enum>(~static_cast<std::underlying_type_t<Enum>>(value)); \
    }                                                            \
    inline constexpr Enum& operator|=(Enum& lhs, Enum rhs) noexcept \
    {                                                               \
        return lhs = lhs | rhs;                                     \
    }                                                               \
    inline constexpr Enum& operator&=(Enum& lhs, Enum rhs) noexcept \
    {                                                               \
        return lhs = lhs & rhs;                                     \
    }
