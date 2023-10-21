#pragma once

#include "fl/db/core/Connection.hpp"

namespace Forward::Database {

    /**
     *   DatabasePool class for managing async connections.
     */
    class DatabasePool final
    {
    public:
        struct Info
        {
            std::string Name;
            std::thread::id ThreadID;

            bool IsSeparate = false;

            Info()
            {
                ThreadID = std::this_thread::get_id();
            }
            Info(std::string_view db_name, bool is_separate = false)
                : Name(db_name)
                , IsSeparate(is_separate)
            {
                ThreadID = std::this_thread::get_id();
            }

            bool operator==(Info const& right) const
            {
                if (!IsSeparate)
                    return Name == right.Name;

                return ThreadID == right.ThreadID
                    && Name == right.Name
                    && IsSeparate == right.IsSeparate;
            }

            explicit operator bool() const& noexcept
            {
                return IsSeparate;
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
                size_t operator()(Info const& right) const
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
            };
        };

    private:
        sql::Driver* driver_ = nullptr;

        std::unordered_map<DatabasePool::Info, Ref<Connection>, DatabasePool::Info::Hash> pool_;
        std::shared_mutex pool_mtx_;

        explicit DatabasePool();

    public:
        /**
         * Call MySQL API driver. This is safe function
         *
         * @return instance of MySQL driver that managed by API
         */
        static sql::Driver* GetDriver();

        /**
         * Retrieves a vector of available connections using a tag
         * @param db_name The tag or identifier for the database
         * @return A vector of references to available database connections
         */
        static std::vector<Ref<Connection>> GetConnections(std::string_view db_name);
  
        /**
         * Retrieves the count of connections
         *
         * @return The count of connections
         */
        static uint32_t GetConnectionCount();
        /**
         * Retrieves the count of currently active connections
         *
         * @return The count of active connections
         */
        static uint32_t GetActiveConnectionCount();

        /**
         * Initializes RAII Style scoped database connection
         *
         * @return Scoped pointer to the database instance
         */
        static Scope<Connection> InitScoped();
        /**
         * Initializes a database connection that can be accessed from multiple threads using tag
         *
         * @param db_name The tag or identifier for the database
         * @return Reference to the database instance
         */
        static Ref<Connection> Init(std::string_view db_name = "");
        /**
         * Initializes a separate database connection that can be accessed ONLY from the thread that created it
         *
         * @param db_name The tag or identifier for the database
         * @return Reference to the database instance
         */
        static Ref<Connection> InitSeparate(std::string_view db_name = "");

        /**
         * Retrieves a database instance by its tag. If no instance is found, returns nullptr.
         *
         * @param db_name The tag or identifier for the database
         * @return Reference to the database instance, or nullptr if not found
         */
        static Ref<Connection> Get(std::string_view db_name = "");
        /**
         * Retrieves a database instance by its tag ONLY from the thread that created that instance.
         * If no instance is found, returns nullptr.
         *
         * @param db_name The tag or identifier for the database
         * @return Reference to the database instance, or nullptr if not found
         */
        static Ref<Connection> GetSeparate(std::string_view db_name = "");

        /**
         * Closes all connections and removes a database instance by its tag
         *
         * @param db_name The tag or identifier for the database
         */
        static void Remove(std::string_view db_name = "");
        /**
         * Closes the connection and removes a database instance by its tag ONLY from the thread that created that instance
         *
         * @param db_name The tag or identifier for the database
         */
        static void RemoveSeparate(std::string_view db_name = "");

        /**
         * Checks if a database exists with the given tag
         *
         * @param db_name The tag or identifier for the database
         * @return True if the database exists, false otherwise
         */
        static bool Has(std::string_view db_name = "");
        /**
         * Checks if a database exists with the given tag, but ONLY from the thread that created that instance
         *
         * @param db_name The tag or identifier for the database
         * @return True if the database exists, false otherwise
         */
        static bool HasSeparate(std::string_view db_name = "");

        /**
         * Closes all database connections
         */
        static void CloseAll();
        /**
         * Closes all database connections with tag
         * 
         * @param db_name database connection tag
         */
        static void CloseAll(std::string_view db_name);

        ~DatabasePool();

        DatabasePool(DatabasePool&&) = delete;
        DatabasePool(DatabasePool const&) = delete;

        DatabasePool& operator=(DatabasePool&&) = delete;
        DatabasePool& operator=(DatabasePool const&) = delete;

    private:
        /**
         * Returns an instance of the database singleton class
         *
         * @return Reference to the singleton database instance
         */
        static DatabasePool& Instance();

        /**
         * Initializes a database connection with the given info
         *
         * @param info Information about the database connection to be initialized
         * @return Reference to the database instance
         */
        static Ref<Connection> InitImpl(DatabasePool::Info const& info);
        /**
         * Retrieves a database instance with the given info
         *
         * @param info Information about the database connection to be retrieved
         * @return Reference to the database instance
         */
        static Ref<Connection> GetImpl(DatabasePool::Info const& info);
        /**
         * Checks if a database exists with the given info
         *
         * @param info Information about the database connection to be checked
         * @return True if the database exists, false otherwise
         */
        static bool HasImpl(DatabasePool::Info const& info);
        /**
         * Closes the connection and removes a database instance with the given info
         *
         * @param info Information about the database connection to be removed
         */
        static void RemoveImpl(DatabasePool::Info const& info);
    };
}
