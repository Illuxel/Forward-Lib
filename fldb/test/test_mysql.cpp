#include "fl/db/Database.hpp"
using namespace Forward;

#include <gtest/gtest.h>

static inline void ConcurentAccessFoo(std::string const& db_name) 
{
    Database::Init(db_name);

    auto db = Database::Get(db_name);

    Database::Remove(db_name);
}

TEST(MySql, ConcurentAccess) {
    
    std::vector<std::thread> connections;

    for (uint8_t i = 0; i < 32; ++i)
        connections.emplace_back(&ConcurentAccessFoo, std::to_string(i));

    for (auto& con : connections)
        con.join();
}

TEST(MySql, Connection) {
    
    auto db = Database::Init("test");

    Exception ec;

    db->Connect("tcp://127.0.0.1:3306", "root", "testing", ec);

    EXPECT_TRUE(db->IsConnected());

    db->SetActiveSchema("forward-db");

    auto result = db->Execute("SELECT * FROM user_info WHERE user_id = ?", &ec, 2);

    EXPECT_TRUE(result);
    EXPECT_TRUE(result && result->first());
    
    db->Close();

    Database::Remove("test");
} 

static inline void ConcurentConnectionFoo(std::string const& db_name) 
{
    std::this_thread::sleep_for(std::chrono::seconds(2));

    auto db = Database::Get(db_name);
    auto result = db->Execute("SELECT * FROM user_info WHERE user_id = ?", 2);

    if (result)
        return;

    if (result->first())
        FL_LOG("QueryData", "true");
}

TEST(MySql, MultiConnection) {
    
    std::vector<Exception> ecs;
    std::vector<std::thread> connections;

    auto db = Database::Init("test");

    db->Connect("tcp://127.0.0.1:3306", "root", "testing");
    db->SetActiveSchema("forward-db");

    for (uint8_t i = 0; i < 32; ++i)
    {
        ecs.emplace_back();
        connections.emplace_back(&ConcurentConnectionFoo, "test");
    }

    for (auto& con : connections)
        con.join();

    Database::Remove("test");
} 