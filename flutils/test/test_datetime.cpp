#include "fl/utils/DateTime.hpp"
using namespace fl;

#include <gtest/gtest.h>

TEST(DateTime, DateTimeToCFormat)
{
    std::string out = DateTime::ConvertToCFormat("DD-MM-YYYY hh-mm-ss");

    EXPECT_STREQ(out.c_str(), "%d-%m-%Y %H-%M-%S");
}

TEST(DateTime, DateTimeNow)
{
    std::string now = DateTime::Now().ToString("DD-MM-YYYY hh-mm-ss");
    std::string latenow = DateTime::Now().ToString("DD-MM-YYYY hh-mm-ss");

    EXPECT_STREQ(now.c_str(), latenow.c_str());
}

#include <optional>

TEST(DateTime, Exception)
{
    std::string now = DateTime::Now().ToString("dafsd   4252  2s");
    std::string latenow = DateTime::Now().ToString("strange text");
}