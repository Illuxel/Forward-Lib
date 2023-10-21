#include "fl/db/Connection.hpp"

namespace Forward::Database {

    Connection::~Connection() {}

    void Connection::SetActiveSchema(std::string_view scheme)
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

    bool Connection::Connect(sql::ConnectOptionsMap const& conn_options)
    {
        Exception ec;
        bool is_conn = Connect(conn_options, ec);

        return is_conn;
    }
    bool Connection::Connect(sql::ConnectOptionsMap const& conn_options, Exception& ec)
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

    bool Connection::Connect(std::string_view host, std::string_view user, std::string_view password)
    {
        Exception ec;
        bool is_conn = Connect(host, user, password, ec);

        return is_conn;
    }
    bool Connection::Connect(std::string_view host, std::string_view user, std::string_view password, Exception& ec)
    {
        if (IsConnected())
        {
            FL_LOG("Connection", "Connection already exist");
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

    std::future<bool> Connection::AsyncConnect(sql::ConnectOptionsMap const& conn_options)
    {
        std::future<bool> result = std::async(
            std::launch::async,
            [&]()
            {
                return this->Connect(conn_options);
            });

        return result;
    }
    std::future<bool> Connection::AsyncConnect(std::string_view host, std::string_view user, std::string_view password)
    {
        std::future<bool> result = std::async(
            std::launch::async, 
            [&]() 
            {
                return Connect(host, user, password);
            });

        return result;
    }

    Result Connection::Execute(std::string_view sql)
    {
        Exception ec;
        auto result = Execute(sql, ec);

        return result;
    }
    Result Connection::Execute(std::string_view sql, Exception& ec)
    {
        if (!IsConnected())
        {
            FL_LOG("Connection", "is not connected");
            return nullptr;
        }
        if (!IsActiveSchema())
        {
            FL_LOG("Connection", "scheme was not set");
            return nullptr;
        }
        
        Result result;

        {
            MySQL::DriverLock d_lock(driver_);

            {
                std::lock_guard lock(conn_mtx_);

                try
                {
                    Query query(connection_->createStatement());
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

    std::future<Result> Connection::AsyncExecute(std::string_view sql)
    {
        std::future<Result> future = std::async(
            std::launch::async,
            [&]() 
            {
                return Execute(sql);
            });

        return future;
    }

    void Connection::Close()
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

    bool Connection::IsValid() const
    {
        std::shared_lock lock(data_mtx_);
        return (bool)connection_;
    }
    bool Connection::IsConnected() const
    {
        if (!IsValid())
            return false;

        MySQL::DriverLock d_lock(driver_);
        std::lock_guard lock(conn_mtx_);

        return connection_->isValid();
    }
    bool Connection::IsActiveSchema() const 
    {
        std::shared_lock lock(data_mtx_);
        return is_scheme;
    }
}