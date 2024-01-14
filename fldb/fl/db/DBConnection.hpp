#pragma once

#include "fl/db/Core.hpp"

namespace Forward::Database {

    template <class Connector>
    class ConnetionOptions {
    private:

    public:
        ConnetionOptions() {}

    };

    /**
     * Represents database connection
     */
    template<class Connector>
    class DBConnection
    {
    public:
        using Options = ConnetionOptions<Connector>;

        using Scope = Scope<Connector>;
        using Ref = Ref<Connector>;

    private:
        Scope conn_ = nullptr;
        mutable std::shared_mutex mtx_;

    public:
        template<typename ...Args> 
        DBConnection(Args&&... args) 
        {
            conn_ = MakeScope(std::forward(args)...);
        }
        ~DBConnection() {}

        /**
         * Connects to existing database using options map
         *
         * @param options is a map with properties (option and value)
         * @param ex callback error
         *
         * @return true if connection success
         */
        bool Connect(Options const& options) 
        {
            Exception ex;
            bool is_conn = Connect(options, ex);

            return is_conn;
        }
        /**
         * Connects to existing database using options map.
         *
         * @param options is a map with properties (option and value)
         * @param ex callback error
         *
         * @return true if connection success
         */
        bool Connect(Options const& options, Exception& ex)
        {
            if (IsConnected())
            {
                FL_LOG("DBConnection", "Connection already exist");
                return true;
            }

            return IsConnected();
        }

        /**
         * Connects to database async using options map
         *
         * @param options is a map with properties (option and value)
         * @return true if connection success
         */
        std::future<bool> ConnectAsync(Options const& options) 
        {
            return std::future<bool>();
        }
        /**
         * Connects to database async using options map and returns errors
         *
         * @param options is a map with properties (option and value)
         * @param ex callback error
         * @return true if connection success
         */
        std::future<bool> ConnectAsync(Options const& options, Exception& ex) 
        {
            return std::future<bool>();
        }

        /**
         * Closes database connection
         */
        bool Close() 
        { 
            return true;
        }
        /**
         * Closes database connection asynchronously
         */
        std::future<bool> CloseAsync() 
        { 
            return std::future<bool>(); 
        }

        /**
         * Checks if database has initialized connection
         *
         * @return true if connection ptr is initialized, otherwise will return false
         */
        bool IsValid() const { return true;  }
        /**
         * Checks if database has established connection
         *
         * @return true if connection exist, otherwise false
         */
        bool IsConnected() const { return true; }

        bool IsTable() const { return true; }
    };
} // namespace Forward::Database
