#pragma once

#include "fl/db/DBConnection.hpp"

namespace Forward {

    /**
     *   Databases class for managing async connections.
     */
    class Database
    {
    public:
        struct DBInfo
        {
            std::string Name;
            std::thread::id ThreadID;

            DBInfo();
            DBInfo(std::string_view db_name);

            bool operator==(DBInfo const& right) const;

            struct Hash
            {
                std::size_t operator()(DBInfo const& right) const noexcept;
            };
        };

    private:
        static sql::Driver* driver_;

        static std::shared_mutex conn_pool_mtx_;
        static std::unordered_map<DBInfo, Ref<DBConnection>, DBInfo::Hash> conn_pool_;

        explicit Database() = default;

    public:
        /**
         * Initializes a database connection and returns a reference to the database instance
         *
         * @param db_name The tag or identifier for the database
         * @return Reference to the database instance
         */
        static Ref<DBConnection> Init(std::string_view db_name = "");

        /**
         * Retrieves a database instance by its tag. If no instance is found, returns nullptr.
         *
         * @param db_name The tag or identifier for the database
         * @return Reference to the database instance, or nullptr if not found
         */
        static Ref<DBConnection> Get(std::string_view db_name = "");
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
        static void InitDriver();

    };
}
