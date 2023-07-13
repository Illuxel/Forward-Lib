#pragma once

#include <type_traits>

namespace Forward {

    template<typename EnumType>
    class Enum
    {
    public:
        static_assert(std::is_enum_v<EnumType>, "Enum class requires an enum type.");
    };
} // namespace Forward
