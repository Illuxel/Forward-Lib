#pragma once

#include "fl/db/Core.hpp"

namespace Forward::Database {
    
    class Configuration 
    {
    private:
        std::map<std::string> config_;

    public:
        Configuration();
        Configuration(std::string_view name);
        Configuration(std::string_view name, );
        Configuration(std::map<std::string> const& config);
    };
} // namespace Forward::Database
