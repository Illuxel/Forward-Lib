#include "fl/db/Database.hpp"
using namespace Forward::Database;

#include <gtest/gtest.h>

TEST(SQLite, Init) 
{
    auto db = Database::Init<SQLiteConnecion>();
}