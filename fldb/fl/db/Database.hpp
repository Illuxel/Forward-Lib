#pragma once

#include "fl/db/core/Connection.hpp"

namespace Forward::Database {

    class SQLiteConnecion;
    class MySQLConnection;

    struct DatabaseInfo
    {
        std::string Name;
        std::thread::id ThreadID;

        bool Separate = false;

        DatabaseInfo()
        {
            ThreadID = std::this_thread::get_id();
        }
        DatabaseInfo(std::string_view db_name, bool is_separate = false)
            : Name(db_name)
            , Separate(is_separate)
        {
            ThreadID = std::this_thread::get_id();
        }

        static DatabaseInfo Make(std::string_view db_name, bool is_separate = false)
        {
            return DatabaseInfo(db_name, is_separate);
        }

        bool operator==(DatabaseInfo const& right) const
        {
            if (!Separate)
                return Name == right.Name;

            return ThreadID == right.ThreadID
                && Name == right.Name
                && Separate == right.Separate;
        }

        explicit operator bool() const& noexcept
        {
            return Separate;
        }
        operator std::string() const& noexcept
        {
            return Name;
        }
        operator std::thread::id() const& noexcept
        {
            return ThreadID;
        }

        struct Hash
        {
            size_t operator()(DatabaseInfo const& right) const
            {
                std::hash<std::thread::id> hash_id;
                std::hash<std::string> hash_name;
                std::hash<bool> hash_separate;

                std::size_t seed = hash_name(right.Name);

                if (right.Separate)
                {
                    seed ^= hash_id(right.ThreadID) << 1;
                    seed ^= hash_separate(right.Separate) << 2;
                }

                return seed;
            }
        };
    };

    /**
     *   Thread save database pool class for storing connections
     */
    template<typename Connector>
    class DatabasePool final 
    {
        using ConnectionRef = DBConnection<Connector>;

    private:
        std::unordered_map<DatabaseInfo, ConnectionRef, DatabaseInfo::Hash> pool_;
        mutable std::shared_mutex mtx_;

    public:
        explicit DatabasePool() {} 

        void Add(std::string_view db_name, ConnectionRef const& connection) 
        {
            auto info = DatabaseInfo::Make(db_name);
            std::unique_lock lock(mtx_);

            pool_.emplace(info, connection);
        }
        void Remove(std::string_view db_name) 
        {
            std::unique_lock lock(mtx_);
        }

        ConnectionRef& At(DatabaseInfo const& info) const 
        {
            std::shared_lock lock(mtx_);
            return pool_.at(info);
        }

        ConnectionRef& Find(DatabaseInfo const& info) 
        {
            std::shared_lock lock(mtx_);
            return true;
        }

        auto begin() { return pool_.begin(); }
        auto end() { return pool_.end(); }

        auto cbegin() { return pool_.cbegin(); }
        auto cend() { return pool_.cend(); }

        ~DatabasePool()
        {
            for (auto conn : pool_)
            {
                conn->Close();
            }
        }
    };

    /**
     *  Database manager class that manages all available connections
     */
    class Database
    {
        template<typename Connector>
        using ConnectionScope = DBConnection<Connector>;

        template<typename Connector>
        using ConnectionRef = DBConnection<Connector>;

        Database() {}

    public:
        /**
         * Retrieves a vector of all database connections using a tag
         * @param db_name The tag or identifier for the database
         * @return A vector of references to all database connections
         */
        template<typename Connector>
        static std::vector<ConnectionRef<Connector>> GetConnections(std::string_view db_name)
        {
            auto& pool = DatabasePool::GetInstance<Connector>();

            std::shared_lock lock(pool.GetMutex());
            std::vector<ConnectionRef<Connector>> conns;

            for (auto const& [info, conn] : pool.data_)
            {
                if (info == db_name)
                {
                    conns.emplace_back(conn);
                }
            }

            return conns;
        }
        /**
         * Retrieves a vector of active database connections using a tag
         * @param db_name The tag or identifier for the database
         * @return A vector of references to active database connections
         */
        template<typename Connector>
        static std::vector<ConnectionRef<Connector>> GetActiveConnections(std::string_view db_name)
        {
            auto& pool = DatabasePool::GetInstance<Connector>();

            std::shared_lock lock(pool.GetMutex());
            std::vector<ConnectionRef<Connector>> conns;

            for (auto const& [info, conn] : pool.data_)
            {
                if (info == db_name && conn->IsActive())
                {
                    conns.emplace_back(conn);
                }
            }

            return conns;
        }

        /**
         * Initializes RAII Style scoped database connection
         *
         * @return Scoped pointer to the database instance
         */
        template<typename Connector>
        static ConnectionScope<Connector> InitScoped()
        {
            return MakeScope<Connection<Connector>>();
        }
        /**
         * Initializes a database connection that can be accessed from multiple threads using tag
         *
         * @param db_name The tag or identifier for the database
         * @return Reference to the database instance
         */
        template<typename Connector>
        static ConnectionRef<Connector> Init(std::string_view db_name = "")
        {
            DatabaseInfo info(db_name);

            return Database::InitImpl<Connector>(info);
        }
        /**
         * Initializes a separate database connection that can be accessed ONLY from the thread that created it
         *
         * @param db_name The tag or identifier for the database
         * @return Reference to the database instance
         */
        template<typename Connector>
        static ConnectionRef<Connector> InitSeparate(std::string_view db_name = "")    
        {
            DatabaseInfo info(db_name, true);

            return Database::InitImpl(info);
        }

        /**
         * Retrieves a database instance by its tag. If no instance is found, returns nullptr.
         *
         * @param db_name The tag or identifier for the database
         * @return Reference to the database instance, or nullptr if not found
         */
        template<typename Connector>
        static ConnectionRef<Connector> Get(std::string_view db_name = "")
        {
            DatabaseInfo info(db_name);

            return Database::GetImpl<Connector>(info);
        }
        /**
         * Retrieves a database instance by its tag ONLY from the thread that created that instance.
         * If no instance is found, returns nullptr.
         *
         * @param db_name The tag or identifier for the database
         * @return Reference to the database instance, or nullptr if not found
         */
        template<typename Connector>
        static ConnectionRef<Connector> GetSeparate(std::string_view db_name = "")
        {
            DatabaseInfo info(db_name, true);

            return Database::GetImpl<Connector>(info);
        }

        /**
         * Closes all connections and removes a database instance by its tag
         *
         * @param db_name The tag or identifier for the database
         */
        template<typename Connector>
        static bool Remove(std::string_view db_name = "")
        {
            DatabaseInfo info(db_name);

            return Database::RemoveImpl(info);
        }
        /**
         * Closes the connection and removes a database instance by its tag ONLY from the thread that created that instance
         *
         * @param db_name The tag or identifier for the database
         */
        template<typename Connector>
        static bool RemoveSeparate(std::string_view db_name = "")
        {
            DatabaseInfo info(db_name, true);

            return Database::RemoveImpl(info);
        }

        /**
         * Checks if a database exists with the given tag
         *
         * @param db_name The tag or identifier for the database
         * @return True if the database exists, false otherwise
         */
        template<typename Connector>
        static bool Has(std::string_view db_name = "")
        {
            DatabaseInfo info(db_name);

            return Database::HasImpl(info);
        }
        /**
         * Checks if a database exists with the given tag, but ONLY from the thread that created that instance
         *
         * @param db_name The tag or identifier for the database
         * @return True if the database exists, false otherwise
         */
        template<typename Connector>
        static bool HasSeparate(std::string_view db_name = "")
        {
            DatabaseInfo info(db_name, true);

            return Database::HasImpl(info);
        }

        /**
         * Closes all database connections
         */
        template<typename Connector>
        static void CloseAll(bool remove = true)
        {
            auto& pool = DatabasePool::GetInstance<Connector>();
            std::unique_lock lock(pool.GetMutex());
        
            for (auto& [info, conn] : pool.data_)
            {
                if (conn->IsConnected())
                {
                    conn->Close();
                }
            }
        }
        /**
         * Closes all database connections with tag
         * 
         * @param db_name database connection tag
         */
        template<typename Connector>
        static void CloseAll(std::string_view db_name, bool remove = true)
        {
            auto& pool = DatabasePool::GetInstance<Connector>();

            for (auto& conn : GetConnections<Connector>(db_name))
            {
                if (conn->IsConnected())
                {
                    conn->Close();
                }
            }
        }

    private:
        /**
         * Initializes a database connection with the given info
         *
         * @param info Information about the database connection to be initialized
         * @return Reference to the database instance
         */
        template<typename Connector>
        static ConnectionRef<Connector> InitImpl(DatabaseInfo const& info)
        {
            auto& pool = GetInstance<Connector>();

            if (Database::HasImpl<Connector>(info))
                return pool.data_[info];

            std::unique_lock lock(pool.GetMutex());

            auto conn = MakeRef<Connection<Connector>>();
            pool.data_.insert(std::make_pair(info, conn));

            return conn;
        }
        
        /**
         * Retrieves a database instance with the given info
         *
         * @param info Information about the database connection to be retrieved
         * @return Reference to the database instance
         */
        template<typename Connector>
        static ConnectionRef<Connector> GetImpl(DatabaseInfo const& info)
        {
            auto& pool = DatabasePool::GetInstance<Connector>();

            if (!DatabasePool::HasImpl<Connector>(info))
                return nullptr;

            std::shared_lock lock(pool.mtx_);

            return pool.data_.at(info);
        }
        
        /**
         * Closes the connection and removes a database instance with the given info
         *
         * @param info Information about the database connection to be removed
         */
        template<typename Connector>
        static bool RemoveImpl(DatabaseInfo const& info)
        {
            auto& pool = DatabasePool::GetInstance<Connector>();
            
            if (!DatabasePool::HasImpl<Connector>(info))
                return false;

            std::unique_lock lock(pool.mtx_);

            pool.data_[info]->Close();
            pool.data_.erase(info);

            return true;
        }

        /**
         * Checks if a database exists with the given info
         *
         * @param info Information about the database connection to be checked
         * @return True if the database exists, false otherwise
         */
        template<typename Connector>
        static bool HasImpl(DatabaseInfo const& info)
        {
            auto& pool = DatabasePool::GetInstance<Connector>();
            std::shared_lock lock(pool.mtx_);
            auto const& it = pool.data_.cfind(info);
            
            return it != pool.data_.cend();
        }
    };
} // namespace Forward::Database
