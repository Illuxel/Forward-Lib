#include "fl/db/Database.hpp"

namespace Forward {
    
    std::shared_mutex Database::s_dbs_mutex{};
    std::unordered_map<std::string, Ref<Database>> Database::s_dbs{};

    Ref<Database> Database::Init(std::string_view db_name)
    {
        if (HasDatabase(db_name))
            return Database::Get(db_name);

        {
            std::unique_lock lock(s_dbs_mutex);

            Ref<Database> db(new Database(sql::mysql::get_driver_instance()));

            s_dbs.insert(std::make_pair(db_name, db));
        }

        return Database::Get(db_name);
    }

    Ref<Database> Database::Get(std::string_view db_name) 
    {
        if (!HasDatabase(db_name))
            return nullptr;

        std::shared_lock lock(s_dbs_mutex);
        return s_dbs[db_name.data()];
    }

    void Database::Remove(std::string_view db_name)
    {
        if (!HasDatabase(db_name))
            return;

        auto db = Database::Get(db_name);

        std::unique_lock lock(s_dbs_mutex);
        s_dbs.erase(db_name.data());
    }

    bool Database::HasDatabase(std::string_view db_name) 
    {
        std::shared_lock lock(s_dbs_mutex);
        return s_dbs.find(db_name.data()) != s_dbs.cend();
    }

    /**
     *   Single connection interface
     */

    Database::Database(sql::mysql::MySQL_Driver* driver)
        : driver_(driver) 
        , is_scheme(false) {}
    Database::~Database() 
    {
        Close();
    }

    bool Database::Connect(std::string_view host, std::string_view user, std::string_view password)
    {
        Exception ec;
        return Connect(host, user, password, ec);
    }
    bool Database::Connect(std::string_view host, std::string_view user, std::string_view password, Exception& ec)
    {
        if (IsConnected())
        {
            FL_LOG("Database", "Connection already exist");
            return true;
        }

        {
            std::unique_lock lock(db_mutex_);

            try
            {
                connection_.reset(driver_->connect(host.data(), user.data(), password.data()));
            }
            catch(Exception const& e)
            {
                ec = e;
                connection_ = nullptr;
            }
        }
        
        return IsConnected();
    }

    Scope<sql::ResultSet> Database::Execute(std::string_view query) const 
    {
        Exception ex;
        return std::move(Execute(query, ex));
    }
    Scope<sql::ResultSet> Database::Execute(std::string_view query, Exception& ex) const
    {
        if (!IsConnected())
        {
            FL_LOG("Database", "is not connected");
            return nullptr;
        }
        if (!IsActiveSchema())
        {
            FL_LOG("Database", "scheme was not set");
            return nullptr;
        }

        std::unique_lock lock(db_mutex_);
        
        try
        {
            Scope<sql::Statement> statement(connection_->createStatement());
            Scope<sql::ResultSet> result(statement->executeQuery(query.data()));

            return std::move(result);
        }
        catch(Exception const& e)
        {
            ex = e;
        }
        
        return nullptr;
    }

    void Database::SetActiveSchema(std::string_view scheme)
    {
        if (!IsConnected())
            return;

        std::unique_lock lock(db_mutex_);

        is_scheme = true;
        connection_->setSchema(scheme.data());
    }

    bool Database::IsConnected() const 
    {
        std::unique_lock lock(db_mutex_);

        if (!connection_) 
            return false;

        return connection_->isValid(); 
    }
    bool Database::IsActiveSchema() const 
    {
        std::shared_lock lock(db_mutex_);
        return is_scheme;
    }

    void Database::Close() 
    {
        if (!IsConnected())
            return;

        std::unique_lock lock(db_mutex_);
        connection_->close();
    }

    void BindValueImpl(Scope<sql::PreparedStatement> const& statement, const uint8_t index, bool value) 
    {
        statement->setBoolean(index, value);
    }
    void BindValueImpl(Scope<sql::PreparedStatement> const& statement, const uint8_t index, int32_t value) 
    {
        statement->setInt(index, value);
    }
    void BindValueImpl(Scope<sql::PreparedStatement> const& statement, const uint8_t index, uint32_t value) 
    {
        statement->setUInt(index, value);
    }
    void BindValueImpl(Scope<sql::PreparedStatement> const& statement, const uint8_t index, int64_t value)
    {
        statement->setInt64(index, value);
    }
    void BindValueImpl(Scope<sql::PreparedStatement> const& statement, const uint8_t index, uint64_t value) 
    {
        statement->setUInt64(index, value);
    }
    void BindValueImpl(Scope<sql::PreparedStatement> const& statement, const uint8_t index, double value) 
    {
        statement->setDouble(index, value);
    }
    void BindValueImpl(Scope<sql::PreparedStatement> const& statement, const uint8_t index, const char* value) 
    {
        statement->setString(index, value);
    }
    void BindValueImpl(Scope<sql::PreparedStatement> const& statement, const uint8_t index, std::string_view value) 
    {
        statement->setString(index, value.data());
    }
    void BindValueImpl(Scope<sql::PreparedStatement> const& statement, const uint8_t index, DateTime const& date) 
    {
        statement->setDateTime(index, date.ToString("YYYY-MM-DD hh:mm:ss"));
    }
    
