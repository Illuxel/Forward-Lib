#include "fl/db/connector/MySQLConnection.hpp"

#include <future>

namespace Forward::Database {

    MySQLConnection::MySQLConnection(Scope<mysqlx::Session> connection)
    {
        if (!connection)
            return;

        conn_ = std::move(connection);
    }
    
    MySQLConnection::MySQLConnection(Options const& options)
    {
        Connect(options);
    }
    MySQLConnection::MySQLConnection(
        std::string_view host, 
        std::string_view user, 
        std::string_view password)
    {
        Exception ex;
        Connect(host, user, password, ex);
    }
    
    void MySQLConnection::SetActiveTable(std::string_view scheme)
    {
        if (!IsConnected())
            return;

        {
            std::lock_guard lock(mtx_);

            try
            {
                mysqlx::Schema table = conn_->createSchema(scheme.data());

                //table.
            }
            catch (Exception const& ex)
            {
                ex_ = ex;
            }
        }
    }

    bool MySQLConnection::Connect(std::string_view host, std::string_view user, std::string_view password)
    {
        Exception ex;
        bool is_conn = Connect(host, user, password, ex);

        return is_conn;
    }
    bool MySQLConnection::Connect(std::string_view host, std::string_view user, std::string_view password, Exception& ex)
    {
        if (IsConnected())
        {
            FL_LOG("MySQLConnection", "Connection already exist");
            return true;
        }

        {
            std::lock_guard lock(mtx_);

            try
            {
                
            }
            catch (std::exception const& e)
            {
                ex = e;
            }
        }

        return IsConnected();
    }

    std::future<bool> MySQLConnection::ConnectAsync(Options const& options)
    {
        std::future<bool> result = 
        std::async(std::launch::async,
        [this, &options]()
        {
            return Connect(options);
        });

        return result;
    }
    std::future<bool> MySQLConnection::ConnectAsync(
        std::string_view host, 
        std::string_view user, 
        std::string_view password)
    {
        std::future<bool> result = 
        std::async(std::launch::async, 
        [this, &host, &user, &password]() 
        {
            Exception ex;
            return Connect(host, user, password);
        });

        return result;
    }

    MySQLConnection::Result MySQLConnection::Execute(std::string_view sql) 
    { 
        return Result(); 
    }

    MySQLConnection::Result MySQLConnection::Execute(
        std::string_view sql, 
        Exception& ex)
    {
        Result result;

        if (!IsConnected())
        {
            FL_LOG("Connection", "is not connected");
            return result;
        }
        if (!IsActiveTable())
        {
            FL_LOG("Connection", "scheme was not set");
            return result;
        }
       
        {
            std::lock_guard lock(mtx_);

            try
            {
                Query query(conn_->createStatement());
                result = std::move(query.Execute(sql.data()));
            }
            catch (std::exception const& e)
            {
                ex = e;
            }
        }

        return result;
    }

    std::future<Result> MySQLConnection::ExecuteAsync(std::string_view sql)
    {
        std::future<Result> future = 
        std::async(std::launch::async,
        [&]() 
        {
            return Execute(sql);
        });

        return future;
    }

    bool MySQLConnection::Close()
    {
        if (!IsConnected())
            return;

        try
        {
            conn_->close();
        }
        catch (std::exception const& e)
        {
            return false;
        }

        return true;
    }


}