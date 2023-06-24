#include "fl/db/Database.hpp"

namespace fl {
    
    std::unordered_map<std::string, Ref<Database>> Database::databases_;

    Database::Database(sql::mysql::MySQL_Driver* driver)
        : is_scheme(false) 
        , driver_(driver)
    {

    }
    Database::~Database() 
    {
        
    }

    Ref<Database> Database::Innit(std::string_view db_name)
    {
        if (HasDatabase(db_name))
            return Get(db_name);

        auto db = new Database(sql::mysql::get_driver_instance());
        databases_.insert(std::make_pair(db_name, std::move(db)));

        return Get(db_name);
    }

    Ref<Database> Database::Get(std::string_view db_name) 
    {
        return databases_.at(db_name.data());
    }
    void Database::Remove(std::string_view db_name)
    {
        if (!HasDatabase(db_name))
            return;

        auto db = databases_.at(db_name.data());
        databases_.erase(db_name.data());
        db->Close();
    }

    bool Database::HasDatabase(std::string_view db_name) 
    {
        return databases_.find(db_name.data()) != databases_.end();
    }

    bool Database::Connect(sql::ConnectOptionsMap options)
    {
        std::exception ec;
        return Connect(options, ec);
    }
    bool Database::Connect(sql::ConnectOptionsMap options, std::exception& ec)
    {
        if (IsConnected())
            return true;

        try
        {
            connection_.reset(driver_->connect(options));
        }
        catch(std::exception e)
        {
            ec = e;
            connection_ = nullptr;
        }

        return IsConnected();
    }

    bool Database::Connect(std::string_view host, 
        std::string_view user, 
        std::string_view password)
    {
        std::exception ec;
        return Connect(host, user, password, ec);
    }
    bool Database::Connect(std::string_view host, 
        std::string_view user, 
        std::string_view password, std::exception& ec)
    {
        if (IsConnected())
            return true;

        try
        {
            connection_.reset(driver_->connect(host.data(), user.data(), password.data()));
        }
        catch(std::exception const& e)
        {
            ec = e;
        }
        
        return IsConnected();
    }

    void Database::Close() const 
    {
        if (!IsConnected())
            return;

        connection_->close();
    }

    void Database::SetActiveSchema(std::string_view scheme)
    {
        if (!IsConnected())
            return;

        is_scheme = true;
        connection_->setSchema(scheme.data());
    }

    Ref<sql::ResultSet> Database::Execute(std::string_view query) const 
    {
        std::exception ex;
        return Execute(query, ex);
    }

    Ref<sql::ResultSet> Database::Execute(std::string_view query, std::exception& ex) const
    {
        Ref<sql::ResultSet> result;

        if (!IsConnected())
        {
            FL_LOG("Database", "is not connected");
            return result;
        }
        if (!is_scheme)
        {
            FL_LOG("Database", "scheme was not set");
            return result;
        }

        Scope<sql::Statement> statement(connection_->createStatement());

        try
        {
            Scope<sql::ResultSet> set(statement->executeQuery(query.data()));
            result = std::move(set);
        }
        catch(const std::exception& e)
        {
            ex = e;
        }
        
        return result;
    }

    bool Database::IsConnected() const 
    {
        if (!connection_) 
            return false;

        return connection_->isValid(); 
    }
    bool Database::IsActiveSchema() const 
    {
        return is_scheme;
    }

    void Database::BindValueImpl(Scope<sql::PreparedStatement> const& statement, int index, bool value) {
        statement->setBoolean(index, value);
    }
    void Database::BindValueImpl(Scope<sql::PreparedStatement> const& statement, int index, int32_t value) {
        statement->setInt(index, value);
    }
     void Database::BindValueImpl(Scope<sql::PreparedStatement> const& statement, int index, uint32_t value) {
        statement->setUInt(index, value);
    }
    void Database::BindValueImpl(Scope<sql::PreparedStatement> const& statement, int index, int64_t value) {
        statement->setInt64(index, value);
    }
    void Database::BindValueImpl(Scope<sql::PreparedStatement> const& statement, int index, uint64_t value) {
        statement->setUInt64(index, value);
    }
    void Database::BindValueImpl(Scope<sql::PreparedStatement> const& statement, int index, double value) {
        statement->setDouble(index, value);
    }
    void Database::BindValueImpl(Scope<sql::PreparedStatement> const& statement, int index, const char* value) {
        statement->setString(index, value);
    }
    void Database::BindValueImpl(Scope<sql::PreparedStatement> const& statement, int index, std::string_view value) {
        statement->setString(index, value.data());
    }
    void Database::BindValueImpl(Scope<sql::PreparedStatement> const& statement, int index, DateTime const& date) {
        statement->setDateTime(index, date.ToString("YYYY-MM-DD hh:mm:ss"));
    }
} // namespace fl::db 