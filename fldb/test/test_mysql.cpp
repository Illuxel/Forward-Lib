#include "fl/db/Database.hpp"
using namespace Forward;

#include <gtest/gtest.h>

#define ADDRESS	"tcp://127.0.0.1:3306"		
#define USER_NAME "root"
#define USER_PASSWORD "testing"

#define DB_NAME "forward-db" 

TEST(MySql, ScopedConnection) {
    
    auto db = Database::InitScoped();

    EXPECT_TRUE(db);

    Exception ec;

    db->Connect(ADDRESS, USER_NAME, USER_PASSWORD, ec);

    EXPECT_FALSE(ec);
    EXPECT_TRUE(!ec && db->IsConnected());

    db->SetActiveSchema(DB_NAME);

    EXPECT_TRUE(db->IsActiveSchema());

    {
        auto result = db->Execute("SELECT * FROM user_info WHERE user_id = ?", ec, 2);

        EXPECT_FALSE(ec);
        EXPECT_TRUE(!ec && result.IsEmpty());
    }

    {
        auto result = db->Execute("SELECT * FROM user_info WHERE user_id = ?", ec, 2);
       
        EXPECT_FALSE(ec);
        EXPECT_TRUE(!ec && result.IsEmpty());
    }
}

static inline void DBPoolMultiAccessFoo(DBTypes::Result& result, Exception& ec)
{
    auto db = Database::InitSeparate();

    if (!db) return;

    db->Connect(ADDRESS, USER_NAME, USER_PASSWORD, ec);
    db->SetActiveSchema(DB_NAME);

    result = std::move(db->Execute("SELECT * FROM user_info WHERE user_id = ?", ec, 2));

    Database::RemoveSeparate();
}

TEST(MySql, DBPoolMultiAccess) {

    const uint8_t thread_count = std::thread::hardware_concurrency();

    std::vector<Exception> ecs;
    std::vector<DBTypes::Result> results;

    ecs.resize(thread_count);
    results.resize(thread_count);

    std::vector<std::thread> connections;

    for (uint8_t i = 0; i < thread_count; ++i)
    {
        auto& ec = ecs[i];
        auto& result = results[i];

        connections.emplace_back(&DBPoolMultiAccessFoo, std::ref(result), std::ref(ec));
    }

    for (auto& con : connections)
    {
        con.join();
    }

    // Wait for completing tasks
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

static inline void SingleObjMultiExecutionFoo(DBTypes::Result& result, Exception& ec)
{
    auto db = Database::Get();

    result = std::move(db->Execute("SELECT * FROM user_info WHERE user_id = ?", ec, 2));
}

TEST(MySql, SingleObjMultiExecution) {

    const uint8_t size = std::thread::hardware_concurrency();

    std::vector<Exception> ecs;
    std::vector<DBTypes::Result> results;

    ecs.resize(size);
    results.resize(size);

    auto db = Database::Init();

    EXPECT_TRUE(db);

    db->Connect(ADDRESS, USER_NAME, USER_PASSWORD);

    EXPECT_TRUE(db->IsConnected());

    db->SetActiveSchema(DB_NAME);

    EXPECT_TRUE(db->IsActiveSchema());

    std::vector<std::thread> executions;

    for (uint8_t i = 0; i < size; ++i)
    {
        auto& ec = ecs[i];
        auto& result = results[i];

        executions.emplace_back(&SingleObjMultiExecutionFoo, std::ref(result), std::ref(ec));
    }

    for (auto& con : executions)
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

    Database::Remove();
} 

TEST(MySql, AsyncConnection) {

    auto db = Database::Init();
    auto is_conn = db->AsyncConnect(ADDRESS, USER_NAME, USER_PASSWORD);

    EXPECT_TRUE(is_conn.valid() && is_conn.get());
}

TEST(MySql, AsyncQueryExecution) {

    auto db = Database::Get();

    EXPECT_TRUE(db->IsConnected());

    db->SetActiveSchema(DB_NAME);

    EXPECT_TRUE(db->IsActiveSchema());

    auto result1 = db->AsyncExecute("SELECT * FROM user_info WHERE user_id = 2");
    auto result2 = db->AsyncExecute("SELECT * FROM user_info WHERE user_id = ?", 2);

    EXPECT_TRUE(result1.valid() && result1.get().IsEmpty());
    EXPECT_TRUE(result2.valid() && result2.get().IsEmpty());

    Database::Remove();
}