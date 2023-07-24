#include "fl/utils/DateTime.hpp"
using namespace std::chrono;

#include "fl/utils/StringBuilder.hpp"

#include <iomanip>
#include <sstream>

namespace Forward {

    DateTime::DateTime()
        : time_point_(std::nullopt)
    {
        time_point_.emplace(system_clock::now());
    }

    DateTime::DateTime(std::chrono::system_clock::time_point time_point)
        : time_point_(std::nullopt)
    {
        time_point_.emplace(time_point);
    }
    DateTime::DateTime(std::string_view time, std::string_view format)
        : time_point_(std::nullopt)
    {
        *this = FromString(time, format);
    }

    DateTime::DateTime(DateTime&& right) noexcept
    {
        time_point_ = std::move(right.time_point_);
    }
    DateTime::DateTime(DateTime const& right) noexcept
    {
        time_point_ = right.time_point_;
    }

    uint8_t DateTime::Day() const
    {
        if (!IsValid())
            return 0;

        return FromChronoToTm(time_point_.value()).tm_mday;
    }
    uint8_t DateTime::Month() const
    {
        if (!IsValid())
            return 0;

        return 1 + FromChronoToTm(time_point_.value()).tm_mon;
    }
    uint32_t DateTime::Year() const
    {
        if (!IsValid())
            return 0;

        return StartYear + FromChronoToTm(time_point_.value()).tm_year;
    }

    uint8_t DateTime::Seconds() const
    {
        if (!IsValid())
            return 0;

        return duration_cast<seconds>(time_point_->time_since_epoch())
            .count();
    }
    uint8_t DateTime::Minutes() const
    {
        if (!IsValid())
            return 0;

        return duration_cast<minutes>(time_point_->time_since_epoch())
            .count();
    }
    uint8_t DateTime::Hours() const
    {
        if (!IsValid())
            return 0;

        return duration_cast<hours>(time_point_->time_since_epoch())
            .count();
    }

    bool DateTime::IsValid() const
    {
        return time_point_.has_value();
    }

    DateTime DateTime::Now()
    {
        return DateTime();
    }

    std::string DateTime::ToString(std::string_view format) const
    {
        if (!IsValid())
            return "";

        if (format.empty())
            return "";

        std::tm dtime = FromChronoToTm(time_point_.value());
        std::string c_format = ConvertToCFormat(format);

        char buffer[MaxFormatSize];

        std::strftime(buffer, MaxFormatSize, c_format.c_str(), &dtime);

        return buffer;
    }

    DateTime DateTime::FromString(std::string_view time, std::string_view format)
    {
        if (format.empty())
            return DateTime();

        std::tm time_info;
        std::istringstream ss(time.data());

        std::string c_format = ConvertToCFormat(format);

        ss >> std::get_time(&time_info, c_format.c_str());

        if (ss.fail())
            return DateTime();

        system_clock::time_point time_point = FromTmToChrono(time_info);

        return DateTime(time_point);
    }

    system_clock::time_point DateTime::FromTmToChrono(std::tm time_info)
    {
        time_t time = std::mktime(&time_info);
        return system_clock::from_time_t(time);
    }
    std::tm DateTime::FromChronoToTm(system_clock::time_point time_point)
    {
        time_t time = system_clock::to_time_t(time_point);
        return *std::localtime(&time);
    }

    std::string DateTime::ConvertToCFormat(std::string_view format) 
    {
        std::vector<StringArg> c_format
        = StringArg::MakeArgs({
            {"HH", "%I"},       // 12-hour format
            {"hh", "%H"},       // 24-hour format
            {"mm", "%M"},       // minutes (0-59)
            {"ss", "%S"},       // seconds (0-59)
            {"MM", "%m"},       // month (1-12)
            {"DD", "%d"},       // day (1-31)
            {"YYYY", "%Y"}      // year (e.g., 2021)
        });

        return StringBuilder(format, c_format);
    }

    bool DateTime::operator==(DateTime const& right) const
    {
        if (!IsValid() && !right.IsValid())
            return false;

        return time_point_.value() == right.time_point_.value();
    }
    bool DateTime::operator==(std::chrono::system_clock::time_point const& right) const
    {
        if (!IsValid())
            return false;

        return time_point_.value() == right;
    }

    DateTime& DateTime::operator=(DateTime&& right) noexcept
    {
        time_point_ = std::move(right.time_point_);

        return *this;
    }
    DateTime& DateTime::operator=(DateTime const& right) noexcept
    {
        time_point_ = right.time_point_;

        return *this;
    }

} // namespace Forward