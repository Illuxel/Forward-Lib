#include "fl/db/DBConnection.hpp"

namespace Forward {

    /**
     *   Connection interface
     */

    DBConnection::DBConnection(sql::Driver* driver)
        : driver_(driver)
        , is_scheme(false)
    {
    }

    DBConnection::DBConnection(sql::Connection* connection)
        : driver_(nullptr)
        , is_scheme(false)
    {
        if (!connection)
            return;

        driver_ = connection->getDriver();
        connection_.reset(connection);

        if (connection_->getSchema().length() != 0)
            is_scheme = true;
    }
    DBConnection::DBConnection(Scope<sql::Connection>&& connection)
        : driver_(nullptr)
        , is_scheme(false) 
    {
        if (!connection)
            return;

        driver_ = connection->getDriver();
        connection_ = std::move(connection);

        if (connection_->getSchema().length() != 0)
            is_scheme = true;
    }

    DBConnection::DBConnection(sql::Driver* driver, sql::ConnectOptionsMap const& connection_opt)
        : driver_(driver)
        , is_scheme(false)
    {
        Connect(connection_opt);
    }
    DBConnection::DBConnection(sql::Driver* driver, 
        std::string_view host, 
        std::string_view user, 
        std::string_view password)
        : driver_(driver)
        , is_scheme(false)
    {
        Connect(host, user, password);
    }

    DBConnection::~DBConnection() {}

    void DBConnection::SetActiveSchema(std::string_view scheme)
    {
        if (!IsConnected())
            return;

        {
            MySQL::DriverLock d_lock(driver_);

            {
                std::lock_guard lock(conn_mtx_);

                try
                {
                    connection_->setSchema(scheme.data());
                }
                catch (std::exception const& e)
                {

                }
            }
        }

        std::unique_lock lock(data_mtx_);
        is_scheme = true;
    }

    bool DBConnection::Connect(sql::ConnectOptionsMap const& conn_options)
    {
        Exception ec;
        bool is_conn = Connect(conn_options, ec);

        return is_conn;
    }
    bool DBConnection::Connect(sql::ConnectOptionsMap const& conn_options, Exception& ec)
    {
        if (IsConnected())
        {
            FL_LOG("DBConnection", "Connection already exist");
            return true;
        }

        {
            MySQL::DriverLock d_lock(driver_);

            {
                std::lock_guard lock(conn_mtx_);

                try
                {
                    auto conn_options_copy = conn_options;
                    connection_.reset(driver_->connect(conn_options_copy));
                }
                catch (std::exception& e)
                {
                    ec = e;
                    connection_ = nullptr;
                }
            }
        }

        return IsConnected();
    }

    bool DBConnection::Connect(std::string_view host, std::string_view user, std::string_view password)
    {
        Exception ec;
        bool is_conn = Connect(host, user, password, ec);

        return is_conn;
    }
    bool DBConnection::Connect(std::string_view host, std::string_view user, std::string_view password, Exception& ec)
    {
        if (IsConnected())
        {
            FL_LOG("DBConnection", "Connection already exist");
            return true;
        }

        {
            MySQL::DriverLock d_lock(driver_);

            {
                std::lock_guard lock(conn_mtx_);

                try
                {
                    connection_.reset(driver_->connect(host.data(), user.data(), password.data()));
                }
                catch (std::exception const& e)
                {
                    ec = e;
                    connection_ = nullptr;
                }
            }
        }

        return IsConnected();
    }

    std::future<bool> DBConnection::AsyncConnect(sql::ConnectOptionsMap const& conn_options)
    {
        std::future<bool> result = std::async(
            std::launch::async,
            [&]()
            {
                return this->Connect(conn_options);
            });

        return result;
    }
    std::future<bool> DBConnection::AsyncConnect(std::string_view host, std::string_view user, std::string_view password)
    {
        std::future<bool> result = std::async(
            std::launch::async, 
            [&]() 
            {
                return Connect(host, user, password);
            });

        return result;
    }

    DBTypes::Result DBConnection::Execute(std::string_view query)
    {
        Exception ec;
        auto result = Execute(query, ec);

        return result;
    }
    DBTypes::Result DBConnection::Execute(std::string_view sql, Exception& ec)
    {
        if (!IsConnected())
        {
            FL_LOG("DBConnection", "is not connected");
            return nullptr;
        }
        if (!IsActiveSchema())
        {
            FL_LOG("DBConnection", "scheme was not set");
            return nullptr;
        }
        
        DBTypes::Result result;

        {
            MySQL::DriverLock d_lock(driver_);

            {
                std::lock_guard lock(conn_mtx_);

                try
                {
                    DBTypes::Query query(connection_->createStatement());
                    result = std::move(query.Execute(sql.data()));
                }
                catch (std::exception const& e)
                {
                    ec = e;
                }
            }
        }

        return result;
    }

    std::future<DBTypes::Result> DBConnection::AsyncExecute(std::string_view query)
    {
        std::future<DBTypes::Result> future = std::async(
            std::launch::async,
            [&]() 
            {
                return Execute(query);
            });

        return future;
    }

    void DBConnection::Close()
    {
        if (!IsConnected())
            return;

        MySQL::DriverLock d_lock(driver_);
        std::lock_guard lock(conn_mtx_);

        try
        {
            connection_->close();
        }
        catch (std::exception const& e)
        {
            FL_LOG("FAIL", "CLOSE");
        }
    }

    bool DBConnection::IsValid() const
    {
        std::shared_lock lock(data_mtx_);
        return (bool)connection_;
    }
    bool DBConnection::IsConnected() const
    {
        if (!IsValid())
            return false;

        MySQL::DriverLock d_lock(driver_);
        std::lock_guard lock(conn_mtx_);

        return connection_->isValid();
    }
    bool DBConnection::IsActiveSchema() const 
    {
        std::shared_lock lock(data_mtx_);
        return is_scheme;
    }
}