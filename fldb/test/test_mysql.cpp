#include "fl/db/Database.hpp"
using namespace Forward;

#include <gtest/gtest.h>

TEST(MySql, Connection) {
    
    auto db = Database::Init("test");

    Exception ec;

    db->Connect("tcp://127.0.0.1:3306", "root", "testing", ec);

    EXPECT_FALSE(ec);
    EXPECT_TRUE(!ec && db->IsConnected());

    db->SetActiveSchema("forward-db");

    {
        auto result = db->Execute("SELECT * FROM user_info WHERE user_id = ?", ec, 2);

        EXPECT_FALSE(ec);
        EXPECT_TRUE(!ec && result->first());
    }

    {
        auto result = db->Execute("SELECT * FROM user_info WHERE user_id = ?", ec, 2);
       
        EXPECT_FALSE(ec);
        EXPECT_TRUE(!ec && result->first());
    }
    
    Database::Remove("test");
} 

 static inline void ConcurrentAccessFoo() 
 {
     auto db1 = Database::Init("test");

     db1->Connect("tcp://127.0.0.1:3306", "root", "testing");
 }

 TEST(MySql, ConcurrentDBAccess) {
    
     std::vector<std::thread> connections;

     for (uint8_t i = 0; i < 32; ++i)
         connections.emplace_back(&ConcurrentAccessFoo);

     for (auto& con : connections)
         con.join();
 }

