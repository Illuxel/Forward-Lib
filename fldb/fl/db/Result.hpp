#pragma once

#include "fl/db/DBConnection.hpp"

namespace Forward::Database {
    
    template<typename Connector>
    class Result
    {
    private:
        Exception ex_;
        DBConnection<Connector> conn_;

    public:
        Result() {}
        Result(Result&& result)
        {
            *this = std::move(right);
        }

        template<class Type>
        constexpr Type Get(uint32_t index) const
        {
            if (index == 0)
                throw std::invalid_argument("Invalid column index");

            if (IsEmpty())
                throw std::runtime_error("Query result is empty");
            
            return Type();
        }

        template<class Type>
        constexpr Type Get(std::string_view column) const
        {
            int index = result_->findColumn(column.data());
            return Get<Type>(index);
        }

        uint32_t RowCount() const 
        {
            return 0;
        }
        uint32_t ColumnCount() const
        {
            return 0;
        }

        bool IsValid() const
        {
            return conn_.IsValid();
        }
        bool IsEmpty() const
        {
            return true;
        }

        Result<Connector>& operator=(Result<Connector>&& right) noexcept
        {
            *this = std::move(right);
            return *this;
        }

        explicit operator bool() const
        {
            return IsEmpty();
        }
    };
} // namespace Forward::Database