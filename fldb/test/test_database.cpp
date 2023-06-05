#include "fl/db/Database.hpp"
using namespace fl::db;

#include <gtest/gtest.h>

TEST(MySQLDB, Test) {
    
    Database::Innit("test");

    auto db = Database::Get("test");

    db->Connect("tcp://127.0.0.1:3306", "root", "testing");

    EXPECT_TRUE(db->IsConnected());

    db->Disconnect();
} 