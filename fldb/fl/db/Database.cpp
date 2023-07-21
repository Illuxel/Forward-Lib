#include "fl/db/Database.hpp"

#include <algorithm>

namespace Forward {

    Database::Info::Info()
        : Name("")
        , IsSeparate(false)
    {
        ThreadID = std::this_thread::get_id();
    }
    Database::Info::Info(std::string_view db_name, bool is_separate)
        : Name(db_name)
        , IsSeparate(is_separate)
    {
        ThreadID = std::this_thread::get_id();
    }

    bool Database::Info::operator==(Database::Info const& right) const
    {
        return ThreadID == right.ThreadID
            && Name == right.Name
            && IsSeparate == right.IsSeparate;
    }

    size_t Database::Info::Hash::operator()(Info const& right) const
    {
        std::hash<std::thread::id> hash_id;
        std::hash<std::string> hash_name;

        std::hash<bool> hash_separate;

        std::size_t seed_id = hash_id(right.ThreadID);
        std::size_t seed_name = hash_name(right.Name);
        std::size_t seed_separate = hash_separate(right.IsSeparate);

        return seed_name ^ (seed_id << 1) ^ (seed_separate << 2);
    }

    /**
     *  Connection instances handler
     */

    Database::Database()
        : driver_(nullptr)
    {
        try
        {
            if (!driver_)
                driver_ = sql::mysql::get_driver_instance();
        }
        catch (std::exception const& e)
        {
            FL_LOG("Database", e.what());
        }
    }
    Database::~Database()
    {
        CloseAll();
    }

    Database& Database::Instance()
    {
        static Database instance;
        return instance;
    }

    sql::Driver* Database::GetDriver()
    {
        Database& db = Database::Instance();
        std::shared_lock lock(db.pool_mtx_);

        return db.driver_;
    }
    std::vector<Ref<DBConnection>> Database::GetConnections()
    {
        Database& db = Database::Instance();
        std::shared_lock lock(db.pool_mtx_);
        std::vector<Ref<DBConnection>> conns;

        std::transform(db.conn_pool_.cbegin(), db.conn_pool_.cend(),
            std::inserter(conns, conns.end()),
            [](auto const& pair) {
                return pair.second;
            });

        return conns;
    }

    Scope<DBConnection> Database::InitScoped()
    {
        auto driver = Database::GetDriver();
        auto conn = MakeScope<DBConnection>(driver);

        return std::move(conn);
    }

    Ref<DBConnection> Database::Init(std::string_view db_name)
    {
        Database::Info info(db_name);

        return Database::InitImpl(info);
    }
    Ref<DBConnection> Database::InitSeparate(std::string_view db_name)
    {
        Database::Info info(db_name, true);

        return Database::InitImpl(info);
    }
    Ref<DBConnection> Database::InitImpl(Database::Info const& info)
    {
        Database& db = Database::Instance();

        if (Database::HasImpl(info))
            return db.conn_pool_[info];

        auto driver = Database::GetDriver();
        std::unique_lock lock(db.pool_mtx_);
        auto conn = MakeRef<DBConnection>(driver);

        db.conn_pool_.insert(std::make_pair(info, conn));

        return conn;
    }

    void Database::Remove(std::string_view db_name)
    {
        Database::Info info(db_name);
        Database::RemoveImpl(info);
    }
    void Database::RemoveSeparate(std::string_view db_name)
    {
        Database::Info info(db_name, true);
        Database::RemoveImpl(info);
    }
    void Database::RemoveImpl(Database::Info const& info)
    {
        if (!Database::HasImpl(info))
            return;

        Database& db = Database::Instance();
        std::unique_lock lock(db.pool_mtx_);

        db.conn_pool_[info]->Close();
        db.conn_pool_.erase(info);
    }

    Ref<DBConnection> Database::Get(std::string_view db_name)
    {
        Database::Info info(db_name);

        return Database::GetImpl(info);
    }
    Ref<DBConnection> Database::GetSeparate(std::string_view db_name)
    {
        Database::Info info(db_name, true);

        return Database::GetImpl(info);
    }
    Ref<DBConnection> Database::GetImpl(Database::Info const& info)
    {
        if (!Database::HasImpl(info))
            return nullptr;

        Database& db = Database::Instance();
        std::shared_lock lock(db.pool_mtx_);

        return db.conn_pool_[info];
    }

    uint32_t Database::GetConnectionCount()
    {
        Database& db = Database::Instance();
        std::shared_lock lock(db.pool_mtx_);

        return db.conn_pool_.size();
    }
    uint32_t Database::GetActiveConnectionCount()
    {
        uint32_t count = 0;

        for (auto& conn : GetConnections())
        {
            if (conn->IsConnected())
            {
                ++count;
            }
        }

        return count;
    }

    bool Database::Has(std::string_view db_name) 
    {
        Database::Info info(db_name);

        return Database::HasImpl(info);
    }
    bool Database::HasSeparate(std::string_view db_name)
    {
        Database::Info info(db_name, true);

        return Database::HasImpl(info);
    }
    bool Database::HasImpl(Database::Info const& info)
    {
        Database& db = Database::Instance();
        std::shared_lock lock(db.pool_mtx_);

        return db.conn_pool_.find(info) != db.conn_pool_.cend();
    }

    void Database::CloseAll(std::string_view db_name)
    {
        Database& db = Database::Instance();

        for (auto& conn : GetConnections())
        {
            if (conn->IsConnected())
            {
                conn->Close();
            }
        }
    }

} // namespace Forward
