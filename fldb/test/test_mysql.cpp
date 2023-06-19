#include "fl/db/Database.hpp"
using namespace fl;

#include <gtest/gtest.h>

TEST(MySql, Test) {
    
    Database::Innit("test");

    auto db = Database::Get("test");

    db->Connect("tcp://127.0.0.1:3306", "root", "testing");

    EXPECT_TRUE(db->IsConnected());

    db->Close();

    Database::Remove("test");
} 