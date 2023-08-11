#include "fl/db/Database.hpp"
using namespace Forward;

#include <gtest/gtest.h>

#include <sqlite3/sqlite3.h>

TEST(SqLite3, Init) 
{
    sqlite3* db;
    int rc;

    // Create/Open the database
    rc = sqlite3_open("example.db", &db);

    EXPECT_TRUE(rc == SQLITE_OK);

    // Do something with the database (e.g., create a table)
    char* err_message = nullptr;
    const char* create_table_sql = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT, age INTEGER);";

    rc = sqlite3_exec(db, create_table_sql, 0, 0, &err_message);

    EXPECT_TRUE(rc == SQLITE_OK); 

    // Close the database connection
    sqlite3_close(db);
}