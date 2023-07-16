#include "fl/db/Database.hpp"

#include <algorithm>

namespace Forward {

    Database::DBInfo::DBInfo()
        : Name("")
    {
        ThreadID = std::this_thread::get_id();
    }
    Database::DBInfo::DBInfo(std::string_view db_name)
        : Name(db_name)
    {
        ThreadID = std::this_thread::get_id();
    }

    bool Database::DBInfo::operator==(DBInfo const& right) const
    {
        return ThreadID == right.ThreadID && Name == right.Name;
    }

    std::size_t Database::DBInfo::Hash::operator()(DBInfo const& right) const noexcept
    {
        std::hash<std::thread::id> id_hash;
        std::hash<std::string> name_hash;

        std::size_t h1 = id_hash(right.ThreadID);
        std::size_t h2 = name_hash(right.Name);

        return h1 ^ (h2 << 1);
    }

    /**
     *  Connection instances handler 
     */

    sql::Driver* Database::driver_ = nullptr;

    std::shared_mutex Database::conn_pool_mtx_{};
    std::unordered_map<Database::DBInfo, Ref<DBConnection>, Database::DBInfo::Hash> Database::conn_pool_{};

    Ref<DBConnection> Database::Init(std::string_view db_name)
    {
        InitDriver();

        DBInfo info(db_name);

        if (HasDatabase(db_name))
            return conn_pool_[info];

        std::unique_lock lock(conn_pool_mtx_);

        try
        {
            Ref<DBConnection> db = MakeRef<DBConnection>(driver_);
            conn_pool_.insert(std::make_pair(info, db));
        }
        catch (std::exception const& e) 
        {
            FL_LOG("Database", e.what());
        }

        return conn_pool_[info];
    }

    std::vector<Ref<DBConnection>> Database::GetConnections()
    {
        std::vector<Ref<DBConnection>> connections;
        std::shared_lock lock(conn_pool_mtx_);

        std::transform(conn_pool_.cbegin(), conn_pool_.cend(), 
        std::inserter(connections, connections.end()),
           [](auto const& pair) {
                return pair.second;
           });

        return connections;
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

    Ref<DBConnection> Database::Get(std::string_view db_name)
    {
        if (!HasDatabase(db_name))
            return nullptr;

        DBInfo info(db_name);
        std::shared_lock lock(conn_pool_mtx_);

        return conn_pool_[info];
    }

    void Database::Remove(std::string_view db_name)
    {
        if (!HasDatabase(db_name))
            return;

        DBInfo info(db_name);
        std::unique_lock lock(conn_pool_mtx_);

        auto& db = conn_pool_[info];

        db->Close();
        conn_pool_.erase(info);
    }

    bool Database::HasDatabase(std::string_view db_name) 
    {
        DBInfo info(db_name);
        std::shared_lock lock(conn_pool_mtx_);

        return conn_pool_.find(info) != conn_pool_.cend();
    }

    void Database::CloseAll(std::string_view db_name)
    {
        DBInfo info(db_name);
        std::unique_lock lock(conn_pool_mtx_);

        for (auto& conn : GetConnections())
        {
            if (conn->IsConnected())
            {

            }
        }
    }

    void Database::InitDriver()
    {
        std::unique_lock lock(conn_pool_mtx_);

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

} // namespace Forward
