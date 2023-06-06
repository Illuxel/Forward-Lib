#include "fl/utils/DateTime.hpp"
#include "fl/utils/StringBuilder.hpp"

#include <map>
using namespace std::chrono;

namespace fl {

    DateTime::DateTime() 
        : time_point_(std::chrono::system_clock::now()) {}
    DateTime::DateTime(DateTime const& right)
        : time_point_(right.time_point_)
        , date_time_(right.date_time_) {}
    DateTime::DateTime(DateTime&& right)
        : time_point_(std::move(right.time_point_))
        , date_time_(std::move(right.date_time_)) {}

    DateTime& DateTime::operator=(DateTime const& right) 
    {
        date_time_ = right.date_time_;
        time_point_ = right.time_point_;
        return *this;
    }

    short DateTime::Seconds() const 
    {
        return duration_cast<std::chrono::seconds>(time_point_.time_since_epoch())
            .count();
    }
    short DateTime::Minutes() const 
    {
        return duration_cast<std::chrono::minutes>(time_point_.time_since_epoch())
            .count();
    }
    short DateTime::Hours() const 
    {
        return duration_cast<std::chrono::hours>(time_point_.time_since_epoch())
            .count();
    }
    
    short DateTime::Day() const 
    {
        CacheTm();
        return date_time_->tm_mday;
    }
    short DateTime::Month() const 
    {
        CacheTm();
        return date_time_->tm_mon + 1;
    }
    size_t DateTime::Year() const 
    {
        CacheTm();
        return date_time_->tm_year + 1900;
    }

    std::string DateTime::ToString() const
    {
        return ToString(default_format);
    }
    std::string DateTime::ToString(std::string_view format) const
    {
        std::string c_format;

        if (format.empty() || format.size() > max_format_size)
            c_format = ConvertToCFormat(default_format);
        else 
            c_format = ConvertToCFormat(format);

        CacheTm();

        char buffer[max_format_size];

        if (std::strftime(buffer, max_format_size, c_format.data(), &date_time_.value()) == 0)
            return "";

        return buffer;
    }

    DateTime DateTime::Now()
    {
        return DateTime();
    }
    std::string DateTime::ConvertToCFormat(std::string_view format) 
    {
        auto c_style_format = StringArg::MakeArgs({
            {"HH", "%I"},       // 12-hour format
            {"hh", "%H"},       // 24-hour format
            {"mm", "%M"},       // minutes (0-59)
            {"ss", "%S"},       // seconds (0-59)
            {"MM", "%m"},       // month (1-12)
            {"dd", "%d"},       // day (1-31)
            {"yyyy", "%Y"}      // year (e.g., 2021)
        });

        StringBuilder build(format, c_style_format);
        return build;
    }

    void DateTime::CacheTm() const 
    {
        if (date_time_.has_value())
            return;

        auto time = system_clock::to_time_t(time_point_);
        date_time_.emplace(*std::localtime(&time));
    }

} // namespace fl