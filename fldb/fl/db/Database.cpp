#include "fl/db/Database.hpp"

#include <algorithm>

namespace Forward {

    Database::SessionInfo::SessionInfo()
        : Name("")
        , IsSeparate(false)
    {
        ThreadID = std::this_thread::get_id();
    }
    Database::SessionInfo::SessionInfo(std::string_view db_name, bool thread_info)
        : Name(db_name)
        , IsSeparate(false)
    {
        ThreadID = std::this_thread::get_id();
    }

    bool Database::SessionInfo::operator==(SessionInfo const& right) const
    {
        return 
            ThreadID == right.ThreadID 
            && Name == right.Name 
            && IsSeparate == right.IsSeparate;
    }

    std::size_t Database::SessionInfo::Hash::operator()(SessionInfo const& right) const noexcept
    {
        std::hash<std::string> hash_name;
        std::hash<std::thread::id> hash_id;

        std::size_t seed_name = hash_name(right.Name);
        std::size_t seed_id = hash_id(right.ThreadID);

        return seed_name ^ (seed_id << 1);
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

    }

    Database& Database::Instance()
    {
        static Database instance;
        return instance;
    }


    Scope<DBConnection> Database::InitScoped()
    {
        Database& db = Database::Instance();

        auto driver = db.GetDriver();

        return std::move(MakeScope<DBConnection>(driver));
    }

    Ref<DBConnection> const& Database::Init(std::string_view db_name)
    {
        Database& db = Database::Instance();

        if (HasDatabase(db_name))
        {
            return db.Get(db_name);
        }

        auto driver = db.GetDriver();
        auto conn = MakeRef<DBConnection>(driver);

        return nullptr;
    }
    Ref<DBConnection> const& Database::InitSeparate(std::string_view db_name)
    {
        Database& db = Database::Instance();

        if (HasDatabase(db_name))
        {
            return db.Get(db_name);
        }

        auto driver = db.GetDriver();
        auto conn = MakeRef<DBConnection>(driver);

        std::unique_lock lock(db.pool_mtx_);

        SessionInfo info(db_name);

        db.conn_pool_.insert(std::make_pair(info, conn));

        return conn;
    }


    void Database::Remove(std::string_view db_name)
    {
        if (!HasDatabase(db_name))
        {
            return;
        }

        Database& db = Database::Instance();

        std::unique_lock lock(db.pool_mtx_);

        SessionInfo info(db_name);
        Ref<DBConnection> conn = db.conn_pool_[info];

        db.conn_pool_.erase(info);
        conn->Close();
    }

    sql::Driver* Database::GetDriver()
    {
        Database& db = Database::Instance();

        std::shared_lock lock(db.pool_mtx_);

        return db.driver_;
    }

    Ref<DBConnection> const& Database::Get(std::string_view db_name)
    {
        if (!HasDatabase(db_name))
            return nullptr;

        Database& db = Database::Instance();

        std::shared_lock lock(db.pool_mtx_);

        SessionInfo info(db_name);

        return db.conn_pool_[info];
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

    uint32_t Database::GetActiveConnectionSize()
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

    bool Database::HasDatabase(std::string_view db_name) 
    {
        Database& db = Database::Instance();

        std::shared_lock lock(db.pool_mtx_);

        SessionInfo info(db_name);

        return db.conn_pool_.find(info) != db.conn_pool_.cend();
    }

    void Database::CloseAll(std::string_view db_name)
    {
        Database& db = Database::Instance();

        std::unique_lock lock(db.pool_mtx_);

        for (auto& conn : GetConnections())
        {
            if (conn->IsConnected())
            {

            }
        }
    }

} // namespace Forward
