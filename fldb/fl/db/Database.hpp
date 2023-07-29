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
                size_t operator()(Info const& right) const;
            };
        };

    private:
        sql::Driver* driver_;
        std::unordered_map<Database::Info, Ref<DBConnection>, Database::Info::Hash> conn_pool_;

        mutable std::shared_mutex conn_pool_mtx_;

        explicit Database();

    public:
        ~Database();

        /**
         * Call MySQL API driver. This is safe function
         *
         * @return intsance of MySQL driver that managed by API
         */
        static sql::Driver* GetDriver();

        /**
         * Retrieves a vector of available connections using a tag
         * @param db_name The tag or identifier for the database
         * @return A vector of references to available database connections
         */
        static std::vector<Ref<DBConnection>> GetConnections(std::string_view db_name);
  
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
        static Scope<DBConnection> InitScoped();
        /**
         * Initializes a database connection that can be accessed from multiple threads using tag
         *
         * @param db_name The tag or identifier for the database
         * @return Reference to the database instance
         */
        static Ref<DBConnection> Init(std::string_view db_name = "");
        /**
         * Initializes a separate database connection that can be accessed ONLY from the thread that created it
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
         * Retrieves a database instance by its tag ONLY from the thread that created that instance.
         * If no instance is found, returns nullptr.
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
         * @param db_name dataabse connection tag
         */
        static void CloseAll(std::string_view db_name);

        Database(Database&&) = delete;
        Database(Database const&) = delete;

        Database& operator=(Database&&) = delete;
        Database& operator=(Database const&) = delete;

    private:
        /**
         * Returns an instance of the database singleton class
         *
         * @return Reference to the singleton database instance
         */
        static Database& Instance();

        /**
         * Initializes a database connection with the given info
         *
         * @param info Information about the database connection to be initialized
         * @return Reference to the database instance
         */
        static Ref<DBConnection> InitImpl(Database::Info const& info);
        /**
         * Retrieves a database instance with the given info
         *
         * @param info Information about the database connection to be retrieved
         * @return Reference to the database instance
         */
        static Ref<DBConnection> GetImpl(Database::Info const& info);
        /**
         * Checks if a database exists with the given info
         *
         * @param info Information about the database connection to be checked
         * @return True if the database exists, false otherwise
         */
        static bool HasImpl(Database::Info const& info);
        /**
         * Closes the connection and removes a database instance with the given info
         *
         * @param info Information about the database connection to be removed
         */
        static void RemoveImpl(Database::Info const& info);
    };
}
