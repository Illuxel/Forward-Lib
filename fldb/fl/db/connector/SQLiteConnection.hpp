#pragma once 

#include "fl/db/Core.hpp"

#include <sqlite3.h>

namespace Forward::Database {

    struct SQLiteConnectorDelete 
    {
        void operator()(sqlite3* sql) 
        {
            sqlite3_close_v2(sql);
        }
    };

    class SQLiteConnection
    {
    public: 


    private:
        int error_;
        Scope<sqlite3, SQLiteConnectorDelete> conn_ = nullptr;

    public:
        SQLiteConnection();

        bool IsValid() const
        {
            return (bool)conn_;
        }
        bool IsConnected() const
        {
            return IsValid();
        }
    };
} // namespace Forward::Database::SQLite