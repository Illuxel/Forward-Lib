#pragma once

#include <ctime>
#include <chrono>

#include <string>
#include <optional>

namespace Forward {
    
    /**
     * Represents date time class with some feautures 
     * 
     * Available format
     * 
     * HH    - 12-hour format
     * hh    - 24-hour format
     * mm    - minutes (0-59)
     * ss    - seconds (0-59)
     * MM    - month (1-12)
     * DD    - day (1-31)
     * YYYY  - year (e.g., 2021)
     */
    class DateTime
    {
    private:
        std::optional<std::chrono::system_clock::time_point> time_point_;

    public:
        static constexpr inline const uint32_t StartYear = 1900;

        static constexpr inline const uint32_t MaxFormatSize = 64;
        static constexpr inline const char* DefaultFormat = "DD-MM-YYYY hh:mm:ss";

        DateTime();

        DateTime(std::chrono::system_clock::time_point time_point);
        DateTime(std::string_view time, std::string_view format = DateTime::DefaultFormat);

        DateTime(DateTime&& right) noexcept;
        DateTime(DateTime const& right) noexcept;

        uint8_t Day() const;
        uint8_t Month() const;
        uint32_t Year() const;

        uint8_t Hours() const;
        uint8_t Minutes() const;
        uint8_t Seconds() const;

        /**
         * Checks if time is valid
         */
        bool IsValid() const;

        /**
         * Returns date time in string using specified format
         *
         * @return date time string
         */
        std::string ToString(std::string_view format = DateTime::DefaultFormat) const;

        /**
         * Returns current date time point
         *
         * @return current system time
         */
        static DateTime Now();

        /**
         * Creates timepoint from string
         * 
         * @param time current string time
         * @param format format for parsing time string
         * 
         * @return current system time
         */ 
        static DateTime FromString(std::string_view time, std::string_view format = DateTime::DefaultFormat);

        /**
         * Converts tm to chrono time point
         *
         * @param time_info
         *
         * @return 
         */
        static std::chrono::system_clock::time_point FromTmToChrono(std::tm time_info);
        /**
         * Converts chrono time point to C tm
         * 
         * @param time_point 
         * 
         * @return 
         */
        static std::tm FromChronoToTm(std::chrono::system_clock::time_point time_point);

        /**
         * HH     -  %I 12-hour format
         * hh     -  %H 24-hour format
         * mm     -  %M minutes (0-59)
         * ss     -  %S seconds (0-59)
         * MM     -  %m month (1-12)
         * DD     -  %d day (1-31)
         * YYYY   -  %Y year (e.g., 2021)
         * 
         * @param format 
         * 
         * @return C format date time
         */
        static std::string ConvertToCFormat(std::string_view format);

        bool operator==(DateTime const& right) const;
        bool operator==(std::chrono::system_clock::time_point const& right) const;

        DateTime& operator=(DateTime&& right) noexcept;
        DateTime& operator=(DateTime const& right) noexcept;

        operator std::chrono::system_clock::time_point() const
        {
            if (!IsValid())
                return std::chrono::system_clock::time_point();

            return time_point_.value();
        }
        operator std::tm() const
        {
            if (!IsValid())
                return std::tm();

            return DateTime::FromChronoToTm(time_point_.value());
        }
    };  
    
} // namespace Forward