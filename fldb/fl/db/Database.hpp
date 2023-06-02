#pragma once

#include "fl/utils/Memory.hpp"

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>

namespace fl::db {

    using sql::Connection;
    using sql::mysql::MySQL_Driver;
    
    class Database
    {
        Scope<MySQL_Driver> driver_;
        std::unordered_map<std::string, Scope<Connection>> connections_;

    private:
        Database() 
            : driver_(sql::mysql::get_driver_instance()) {}
    public:
        static Ref<Database> Get(std::string_view name);


        Database(const Database&) = delete;
        Database& operator=(const Database&) = delete;
    };
}