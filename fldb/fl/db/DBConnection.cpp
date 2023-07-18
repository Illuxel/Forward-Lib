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

        driver_->threadInit();

        {
            std::lock_guard lock(conn_mtx_);
            connection_->setSchema(scheme.data());
        }

        driver_->threadEnd();

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

        driver_->threadInit();

        {
            std::lock_guard lock(conn_mtx_);

            try
            {
                auto conn_options_copy = conn_options;
                connection_.reset(driver_->connect(conn_options_copy));
            }
            catch (std::exception& e)
            {
                ec.SetError(e);
                connection_ = nullptr;
            }
        }

        driver_->threadEnd();

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

        driver_->threadInit();

        {
            std::lock_guard lock(conn_mtx_);

            try
            {
                connection_.reset(driver_->connect(host.data(), user.data(), password.data()));
            }
            catch (std::exception& e)
            {
                ec.SetError(e);
                connection_ = nullptr;
            }    
        }

        driver_->threadEnd();

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

    Query::Result DBConnection::Execute(std::string_view query)
    {
        Exception ex;
        auto result = Execute(query, ex);

        return std::move(result);
    }
    Query::Result DBConnection::Execute(std::string_view query, Exception& ec)
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
        
        Query::Result result;

        driver_->threadInit();

        {
            std::lock_guard lock(conn_mtx_);

            try
            {
                Query::Executable statement(connection_->createStatement());
                result.reset(statement->executeQuery(query.data()));
            }
            catch (std::exception const& e)
            {
                ec.SetError(e);
                result = nullptr;
            }
        }

        driver_->threadEnd();

        return std::move(result);
    }

    std::future<Query::Result> DBConnection::AsyncExecute(std::string_view query)
    {
        std::future<Query::Result> future = std::async(
            std::launch::async,
            [&]() 
            {
                Query::Result result = Execute(query);

                return std::move(result);
            });

        return future;
    }

    void DBConnection::Close()
    {
        if (!IsConnected())
            return;

        driver_->threadInit();

        {
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

        driver_->threadEnd();
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

        bool is_valid;

        driver_->threadInit();

        {
            std::lock_guard lock(conn_mtx_);
            is_valid = connection_->isValid();
        }

        driver_->threadEnd();

        return is_valid;
    }
    bool DBConnection::IsActiveSchema() const 
    {
        std::shared_lock lock(data_mtx_);
        return is_scheme;
    }

    void DBConnection::BindValueImpl(sql::PreparedStatement* statement, uint32_t const index, bool value) 
    {
        statement->setBoolean(index, value);
    }
    void DBConnection::BindValueImpl(sql::PreparedStatement* statement, uint32_t const index, int32_t value) 
    {
        statement->setInt(index, value);
    }
    void DBConnection::BindValueImpl(sql::PreparedStatement* statement, uint32_t const index, uint32_t value) 
    {
        statement->setUInt(index, value);
    }
    void DBConnection::BindValueImpl(sql::PreparedStatement* statement, uint32_t const index, int64_t value)
    {
        statement->setInt64(index, value);
    }
    void DBConnection::BindValueImpl(sql::PreparedStatement* statement, uint32_t const index, uint64_t value) 
    {
        statement->setUInt64(index, value);
    }
    void DBConnection::BindValueImpl(sql::PreparedStatement* statement, uint32_t const index, double value) 
    {
        statement->setDouble(index, value);
    }
    void DBConnection::BindValueImpl(sql::PreparedStatement* statement, uint32_t const index, const char* value) 
    {
        statement->setString(index, value);
    }
    void DBConnection::BindValueImpl(sql::PreparedStatement* statement, uint32_t const index, std::string_view value) 
    {
        statement->setString(index, value.data());
    }
    void DBConnection::BindValueImpl(sql::PreparedStatement* statement, uint32_t const index, DateTime const& date) 
    {
        statement->setDateTime(index, date.ToString("YYYY-MM-DD hh:mm:ss"));
    }
}