#include "fl/db/Database.hpp"

#include <algorithm>

namespace Forward {

    Database::Info::Info()
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
        if (!IsSeparate)
            return Name == right.Name;

        return ThreadID == right.ThreadID
            && Name == right.Name
            && IsSeparate == right.IsSeparate;
    }

    size_t Database::Info::Hash::operator()(Info const& right) const
    {
        std::hash<std::thread::id> hash_id;
        std::hash<std::string> hash_name;
        std::hash<bool> hash_separate;

        std::size_t seed = hash_name(right.Name);

        if (right.IsSeparate)
        {
            seed ^= hash_id(right.ThreadID) << 1;
            seed ^= hash_separate(right.IsSeparate) << 2;
        }

        return seed;
    }

    /**
     *  Connection instances handler
     */

    Database::Database()
        : driver_(nullptr)
    {
        try
        {
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
        std::shared_lock lock(db.conn_pool_mtx_);

        return db.driver_;
    }

    std::vector<Ref<DBConnection>> Database::GetConnections(std::string_view db_name)
    {
        Database& db = Database::Instance();
        std::shared_lock lock(db.conn_pool_mtx_);
        std::vector<Ref<DBConnection>> conns;

        conns.reserve(db.conn_pool_.size());

        for (auto const& [info, conn] : db.conn_pool_)
        {
            if (info == db_name)
            {
                conns.emplace_back(conn);
            }
        }

        return conns;
    }

    uint32_t Database::GetConnectionCount()
    {
        Database& db = Database::Instance();
        std::shared_lock lock(db.conn_pool_mtx_);

        return db.conn_pool_.size();
    }
    uint32_t Database::GetActiveConnectionCount()
    {
        Database& db = Database::Instance();
        std::shared_lock lock(db.conn_pool_mtx_);

        return std::count_if(db.conn_pool_.cbegin(), db.conn_pool_.cend(),
            [](auto const& connection)
            {
                auto& [info, conn] = connection;

                return conn->IsValid() && conn->IsConnected();
            });
    }

    Scope<DBConnection> Database::InitScoped()
    {
        auto driver = Database::GetDriver();

        return MakeScope<DBConnection>(driver);
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

        std::unique_lock lock(db.conn_pool_mtx_);

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
        Database& db = Database::Instance();
        
        if (!Database::HasImpl(info))
            return;

        std::unique_lock lock(db.conn_pool_mtx_);

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
        Database& db = Database::Instance();

        if (!Database::HasImpl(info))
            return nullptr;

        std::shared_lock lock(db.conn_pool_mtx_);

        return db.conn_pool_.at(info);
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
        std::shared_lock lock(db.conn_pool_mtx_);

        auto const& it = db.conn_pool_.find(info);

        return it != db.conn_pool_.cend();
    }

    void Database::CloseAll()
    {
        Database& db = Database::Instance();
        
        std::unique_lock lock(db.conn_pool_mtx_);
     
        for (auto& [info, conn] : db.conn_pool_)
        {
            if (conn->IsConnected())
            {
                conn->Close();
            }
        }
    }
    void Database::CloseAll(std::string_view db_name)
    {
        Database& db = Database::Instance();

        for (auto& conn : GetConnections(db_name))
        {
            if (conn->IsConnected())
            {
                conn->Close();
            }
        }
    }

} // namespace Forward
