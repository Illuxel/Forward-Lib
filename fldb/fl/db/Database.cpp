#include "fl/db/Database.hpp"

#include <algorithm>

namespace Forward::Database {

    DatabasePool::DatabasePool()
        : driver_(nullptr)
    {
        try
        {
            driver_ = sql::mysql::get_driver_instance();
        }
        catch (std::exception const& e)
        {
            FL_LOG("DatabasePool", e.what());
        }
    }
    DatabasePool::~DatabasePool()
    {
        CloseAll();
    }

    DatabasePool& DatabasePool::Instance()
    {
        static DatabasePool instance;
        return instance;
    }

    sql::Driver* DatabasePool::GetDriver()
    {
        DatabasePool& db = DatabasePool::Instance();
        std::shared_lock lock(db.pool_mtx_);

        return db.driver_;
    }

    std::vector<Ref<DBConnection>> DatabasePool::GetConnections(std::string_view db_name)
    {
        DatabasePool& db = DatabasePool::Instance();
        std::shared_lock lock(db.pool_mtx_);
        std::vector<Ref<DBConnection>> conns;

        conns.reserve(db.pool_.size());

        for (auto const& [info, conn] : db.pool_)
        {
            if (info == db_name)
            {
                conns.emplace_back(conn);
            }
        }

        return conns;
    }

    uint32_t DatabasePool::GetConnectionCount()
    {
        DatabasePool& db = DatabasePool::Instance();
        std::shared_lock lock(db.pool_mtx_);

        return db.pool_.size();
    }
    uint32_t DatabasePool::GetActiveConnectionCount()
    {
        DatabasePool& db = DatabasePool::Instance();
        std::shared_lock lock(db.pool_mtx_);

        return std::count_if(db.pool_.cbegin(), db.pool_.cend(),
            [](auto const& connection)
            {
                auto& [info, conn] = connection;

                return conn->IsValid() && conn->IsConnected();
            });
    }

    Scope<DBConnection> DatabasePool::InitScoped()
    {
        auto driver = DatabasePool::GetDriver();

        return MakeScope<DBConnection>(driver);
    }

    Ref<DBConnection> DatabasePool::Init(std::string_view db_name)
    {
        DatabasePool::Info info(db_name);

        return DatabasePool::InitImpl(info);
    }
    Ref<DBConnection> DatabasePool::InitSeparate(std::string_view db_name)
    {
        DatabasePool::Info info(db_name, true);

        return DatabasePool::InitImpl(info);
    }
    Ref<DBConnection> DatabasePool::InitImpl(DatabasePool::Info const& info)
    {
        DatabasePool& db = DatabasePool::Instance();

        if (DatabasePool::HasImpl(info))
            return db.pool_[info];

        auto driver = DatabasePool::GetDriver();

        std::unique_lock lock(db.pool_mtx_);

        auto conn = MakeRef<DBConnection>(driver);
        db.pool_.insert(std::make_pair(info, conn));

        return conn;
    }

    void DatabasePool::Remove(std::string_view db_name)
    {
        DatabasePool::Info info(db_name);
        DatabasePool::RemoveImpl(info);
    }
    void DatabasePool::RemoveSeparate(std::string_view db_name)
    {
        DatabasePool::Info info(db_name, true);
        DatabasePool::RemoveImpl(info);
    }
    void DatabasePool::RemoveImpl(DatabasePool::Info const& info)
    {
        DatabasePool& db = DatabasePool::Instance();
        
        if (!DatabasePool::HasImpl(info))
            return;

        std::unique_lock lock(db.pool_mtx_);

        db.pool_[info]->Close();
        db.pool_.erase(info);
    }

    Ref<DBConnection> DatabasePool::Get(std::string_view db_name)
    {
        DatabasePool::Info info(db_name);

        return DatabasePool::GetImpl(info);
    }
    Ref<DBConnection> DatabasePool::GetSeparate(std::string_view db_name)
    {
        DatabasePool::Info info(db_name, true);

        return DatabasePool::GetImpl(info);
    }
    Ref<DBConnection> DatabasePool::GetImpl(DatabasePool::Info const& info)
    {
        DatabasePool& db = DatabasePool::Instance();

        if (!DatabasePool::HasImpl(info))
            return nullptr;

        std::shared_lock lock(db.pool_mtx_);

        return db.pool_.at(info);
    }

    bool DatabasePool::Has(std::string_view db_name) 
    {
        DatabasePool::Info info(db_name);

        return DatabasePool::HasImpl(info);
    }
    bool DatabasePool::HasSeparate(std::string_view db_name)
    {
        DatabasePool::Info info(db_name, true);

        return DatabasePool::HasImpl(info);
    }
    bool DatabasePool::HasImpl(DatabasePool::Info const& info)
    {
        DatabasePool& db = DatabasePool::Instance();
        std::shared_lock lock(db.pool_mtx_);

        auto const& it = db.pool_.find(info);

        return it != db.pool_.cend();
    }

    void DatabasePool::CloseAll()
    {
        DatabasePool& db = DatabasePool::Instance();
        
        std::unique_lock lock(db.pool_mtx_);
     
        for (auto& [info, conn] : db.pool_)
        {
            if (conn->IsConnected())
            {
                conn->Close();
            }
        }
    }
    void DatabasePool::CloseAll(std::string_view db_name)
    {
        DatabasePool& db = DatabasePool::Instance();

        for (auto& conn : GetConnections(db_name))
        {
            if (conn->IsConnected())
            {
                conn->Close();
            }
        }
    }

} // namespace Forward
