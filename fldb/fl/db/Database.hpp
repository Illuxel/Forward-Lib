#pragma once

#include "fl/db/DBConnection.hpp"

namespace Forward {

    /**
     *   Databases class for managing async connections.
     */
    class Database final
    {
    public:
        struct Info
        {
            std::string Name;
            std::thread::id ThreadID;

            bool IsSeparate;

            Info();
            Info(std::string_view db_name, bool is_separate = false);

            bool operator==(Database::Info const& right) const;

            operator bool() const&
            {
                return IsSeparate;
            }
            operator std::string() const&
            {
                return Name;
            }
            operator std::thread::id() const&
            {
                return ThreadID;
            }

            struct Hash
            {
                size_t operator()(Info const& right) const;
            };
        };

    private:
        sql::Driver* driver_;
        std::unordered_map<Database::Info, Ref<DBConnection>, Database::Info::Hash> conn_pool_;

        mutable std::shared_mutex pool_mtx_;

        explicit Database();

    public:
        ~Database();

        /**
         * Call MySQL driver 
         *
         * @return 
         */
        static sql::Driver* GetDriver();
        /**
         * Retrieves a vector of available database connections
         *
         * @return A vector of references to available database connections
         */
        static std::vector<Ref<DBConnection>> GetConnections();
  
        /**
         * Retrieves the count of connections
         *
         * @return The count of active connections
         */
        static uint32_t GetConnectionCount();
        /**
         * Retrieves the count of currently active connections
         *
         * @return The count of active connections
         */
        static uint32_t GetActiveConnectionCount();

        /**
         * Initializes scoped database connection
         *
         * @return Scoped ptr to the database instance
         */
        static Scope<DBConnection> InitScoped();

        /**
         * Initializes a database connection that can be accessed from multiple threads
         *
         * @param db_name The tag or identifier for the database
         * @return Reference to the database instance
         */
        static Ref<DBConnection> Init(std::string_view db_name = "");
        /**
         * Initializes a separate database connection that can be accessed ONLY from called thread
         *
         * @param db_name The tag or identifier for the database
         * @return Reference to the database instance
         */
        static Ref<DBConnection> InitSeparate(std::string_view db_name = "");

        /**
         * Retrieves a database instance by its tag. If no instance is found, returns nullptr.
         *
         * @param db_name The tag or identifier for the database
         * @return Reference to the database instance, or nullptr if not found
         */
        static Ref<DBConnection> Get(std::string_view db_name = "");
        /**
         * 
         *
         * @param db_name The tag or identifier for the database
         * @return Reference to the database instance, or nullptr if not found
         */
        static Ref<DBConnection> GetSeparate(std::string_view db_name = "");

        /**
         * Closes all connections and removes a database instance by its tag
         *
         * @param db_name The tag or identifier for the database
         */
        static void Remove(std::string_view db_name = "");
        static void RemoveSeparate(std::string_view db_name = "");

        /**
         * Checks if a database exists with the given tag
         *
         * @param db_name The tag or identifier for the database
         * @return True if the database exists, false otherwise
         */
        static bool Has(std::string_view db_name = "");
        static bool HasSeparate(std::string_view db_name = "");

        /**
         * Closes all named database connections
         */
        static void CloseAll(std::string_view db_name = "");

        Database(Database&&) = delete;
        Database(Database const&) = delete;

        Database& operator=(Database&&) = delete;
        Database& operator=(Database const&) = delete;

    private:
        /**
         * Returns an intance of database singleton class
         *
         * @return Reference to the singleton database instance
         */
        static Database& Instance();

        /**
         * @param db_name The tag or identifier for the database
         * @param separate Boolean that defines wether init connection should be callable ONLY from one thread
         * 
         * @return Reference to the database instance
         */
        static Ref<DBConnection> InitImpl(Database::Info const& info);
        static Ref<DBConnection> GetImpl(Database::Info const& info);

        static bool HasImpl(Database::Info const& info);

        static void RemoveImpl(Database::Info const& info);
    };
}
