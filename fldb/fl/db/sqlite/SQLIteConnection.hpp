#pragma once 

#include <sqlite3/sqlite3.h>

namespace Forward::Database::SQLite {

    struct SQLiteConnectionDelete 
    {
        void operator()(sqlite3* sql) 
        {
            sqlite3_close_v2(sql);
        }
    };

    class SQLiteConnection 
    {
    private:
        Scope<sqlite3, SQLiteConnectionDelete> conn_ = nullptr;

    public:

    };
}