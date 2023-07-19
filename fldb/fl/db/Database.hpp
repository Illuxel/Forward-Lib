#pragma once

#include "fl/db/DBConnection.hpp"

namespace Forward {

    /**
     *   Databases class for managing async connections.
     */
    class Database final
    {
    public:
        struct SessionInfo
        {
            std::string Name;
            std::thread::id ThreadID;

            bool IsSeparate;

            SessionInfo();
            SessionInfo(std::string_view db_name, bool is_separate = false);

            bool operator==(SessionInfo const& right) const;

            operator bool() const &
            {
                return IsSeparate;
            }
            operator std::string() const &
            {
                return Name;
            }
            operator std::thread::id() const &
            {
                return ThreadID;
            }

            struct Hash
            {
                std::size_t operator()(SessionInfo const& right) const noexcept;
            };
        };

    private:
        sql::Driver* driver_;

        std::shared_mutex pool_mtx_;
        std::unordered_map<SessionInfo, Ref<DBConnection>, SessionInfo::Hash> conn_pool_;

        explicit Database();

    public:
        ~Database();

        /**
         * Retrieves a database instance by its tag. If no instance is found, returns nullptr.
         *
         * @param db_name The tag or identifier for the database
         * @return Reference to the database instance, or nullptr if not found
         */
        static sql::Driver* GetDriver();
        /**
         * Retrieves a database instance by its tag. If no instance is found, returns nullptr.
         *
         * @param db_name The tag or identifier for the database
         * @return Reference to the database instance, or nullptr if not found
         */
        static Ref<DBConnection> const& Get(std::string_view db_name = "");
        /**
         * Retrieves a vector of available database connections
         *
         * @return A vector of references to available database connections
         */
        static std::vector<Ref<DBConnection>> GetConnections();
        /**
         * Retrieves the count of currently active connections
         *
         * @return The count of active connections
         */
        static uint32_t GetActiveConnectionSize();

        /**
         * Initializes scoped database connection. Destroys after exits scope
         *
         * @return Scoped ptr to the database instance
         */
        static Scope<DBConnection> InitScoped();

        /**
         * Initializes a database connection and returns a reference to the database instance
         *
         * @param db_name The tag or identifier for the database
         * @return Reference to the database instance
         */
        static Ref<DBConnection> const& Init(std::string_view db_name = "");
        /**
         * Initializes a database connection that can be called ONLY from thread that used method InitSeparate().
         *
         * @param db_name The tag or identifier for the database
         * @return Reference to the database instance
         */
        static Ref<DBConnection> const& InitSeparate(std::string_view db_name = "");

        /**
         * Closes all connections and removes a database instance by its tag
         *
         * @param db_name The tag or identifier for the database
         */
        static void Remove(std::string_view db_name = "");

        /**
         * Checks if a database exists with the given tag
         *
         * @param db_name The tag or identifier for the database
         * @return True if the database exists, false otherwise
         */
        static bool HasDatabase(std::string_view db_name = "");

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

    };
}
