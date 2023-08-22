#pragma once

#include <memory>

namespace Forward {
    
    template<class Type>
    using Scope = std::unique_ptr<Type>;

    template<typename Type, typename ...Args>
    constexpr Scope<Type> MakeScope(Args&& ...args)
    {
        return std::make_unique<Type>(std::forward<Args>(args)...);
    }

    template<class Type>
    using Ref = std::shared_ptr<Type>;

    template<class Type, typename ...Args>
    constexpr Ref<Type> MakeRef(Args&& ...args)
    {
        return std::make_shared<Type>(std::forward<Args>(args)...);
    }

} // namespace Forward
