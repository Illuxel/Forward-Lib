#pragma once

#include <chrono>
#include <ctime>

#include <optional>

namespace fl {
    
    class DateTime
    {
        std::chrono::system_clock::time_point time_point_;
        mutable std::optional<std::tm> date_time_;
        
    public:
        static constexpr inline const char* default_format = "dd/MM/yyyy hh:mm:ss";
        static constexpr inline const size_t max_format_size = 64;

        DateTime();
        DateTime(DateTime const& right);
        DateTime(DateTime&& right);

        DateTime& operator=(DateTime const& right);

        short Hours() const;
        short Minutes() const;
        short Seconds() const;

        short Day() const;
        short Month() const;
        size_t Year() const;

        std::string ToString() const;
        /**
         *   HH    - 12-hour format
         *   hh    - 24-hour format
         *   mm    - minutes (0-59)
         *   ss    - seconds (0-59)
         *   MM    - month (1-12)
         *   dd    - day (1-31)
         *   yyyy  - year (e.g., 2021)
         */
        std::string ToString(std::string_view format) const;
        /**
         *  return current system time
         */   
        static DateTime Now();
        /**
         *   HH     -  %I 12-hour format
         *   hh     -  %H 24-hour format
         *   mm     -  %M minutes (0-59)
         *   ss     -  %S seconds (0-59)
         *   MM     -  %m month (1-12)
         *   dd     -  %d day (1-31)
         *   yyyy   -  %Y year (e.g., 2021)
         * 
         *   returns c format datetime
         */
        static std::string ConvertToCFormat(std::string_view format);

    private:
        void DateTime::CacheTm() const;

    };  
    
} // namespace fl