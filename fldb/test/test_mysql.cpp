#include "fl/db/Database.hpp"
using namespace Forward;

#include <gtest/gtest.h>

TEST(MySql, ScopedConnection) {
    
    auto db = Database::InitScoped();

    EXPECT_TRUE(db);

    Exception ec;

    db->Connect("tcp://127.0.0.1:3306", "root", "testing", ec);

    EXPECT_FALSE(ec);
    EXPECT_TRUE(!ec && db->IsConnected());

    db->SetActiveSchema("forward-db");

    EXPECT_TRUE(db->IsActiveSchema());

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
} 

static inline void DBPoolMultiAccessFoo(Query::Result& result, Exception& ec)
{
    auto db = Database::InitSeparate();

    if (!db) return;

    db->Connect("tcp://127.0.0.1:3306", "root", "testing", ec);
    db->SetActiveSchema("forward-db");

    result = std::move(db->Execute("SELECT * FROM user_info WHERE user_id = ?", ec, 2));

    Database::Remove();
}

TEST(MySql, DBPoolMultiAccess) {

    const uint8_t size = std::thread::hardware_concurrency();

    std::vector<Exception> ecs;
    std::vector<Query::Result> results;

    ecs.resize(size);
    results.resize(size);

    std::vector<std::thread> connections;

    for (uint8_t i = 0; i < size; ++i)
    {
        auto& ec = ecs[i];
        auto& result = results[i];

        connections.emplace_back(&DBPoolMultiAccessFoo, std::ref(result), std::ref(ec));
    }

    for (auto& con : connections)
    {
        con.join();
    }

    std::this_thread::sleep_for(std::chrono::seconds(4));

    for (auto const& ec : ecs)
    {
        EXPECT_FALSE(ec);
    }

    for (auto& result : results)
    {
        EXPECT_TRUE(result);
    }
}

TEST(MySql, AsyncConnection) {

    auto db = Database::InitScoped();

    EXPECT_TRUE(db);

    auto is_conn = db->AsyncConnect("tcp://127.0.0.1:3306", "root", "testing");

    EXPECT_TRUE(is_conn.valid() && is_conn.get());
}

TEST(MySql, AsyncQueryExecution) {

    auto db = Database::InitScoped();

    EXPECT_TRUE(db);

    auto is_conn = db->AsyncConnect("tcp://127.0.0.1:3306", "root", "testing");

    EXPECT_TRUE(is_conn.valid() && is_conn.get());

    db->SetActiveSchema("forward-db");

    auto result1 = db->AsyncExecute("SELECT * FROM user_info WHERE user_id = 2");
    auto result2 = db->AsyncExecute("SELECT * FROM user_info WHERE user_id = ?", 2);

    EXPECT_TRUE(result1.valid() && result1.get());
    EXPECT_TRUE(result2.valid() && result2.get());
}