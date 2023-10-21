#include "fl/db/mysql/MySQLConnection.hpp"

namespace Forward::Database::MySQL {

	MySQLConnection::MySQLConnection(sql::Driver* driver)
        : driver_(driver)
    {
    }

    MySQLConnection::MySQLConnection(sql::Connection* connection)
    {
        if (!connection)
            return;

        driver_ = connection->getDriver();
        conn_.reset(connection);

        if (conn_->getSchema().length() != 0)
            is_scheme = true;
    }
    MySQLConnection::MySQLConnection(Scope<sql::Connection>&& connection)
    {
        if (!connection)
            return;

        driver_ = connection->getDriver();
        conn_ = std::move(connection);

        if (conn_->getSchema().length() != 0)
            is_scheme = true;
    }
	
	MySQLConnection::MySQLConnection(sql::Driver* driver, sql::ConnectOptionsMap const& connection_opt)
        : driver_(driver)
    {
        Connect(connection_opt);
    }
    MySQLConnection::MySQLConnection(
		sql::Driver* driver, 
        std::string_view host, 
        std::string_view user, 
        std::string_view password)
        : driver_(driver)
    {
        Connect(host, user, password);
    }

}