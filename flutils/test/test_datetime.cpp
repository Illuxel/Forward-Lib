#include "fl/utils/DateTime.hpp"

#include <gtest/gtest.h>

TEST(DateTime, DateTimeToCFormat)
{
    using namespace fl::utils;

    std::string out = DateTime::ConvertToCFormat("dd-MM-yyyy hh-mm-ss");

    EXPECT_STREQ(out.c_str(), "%d-%m-%Y %H-%M-%S");
}

TEST(DateTime, DateTimeNow)
{
    using namespace fl::utils;

    std::string now = DateTime::Now().ToString("dd-MM-yyyy hh-mm-ss");
    std::string latenow = DateTime::Now().ToString("dd-MM-yyyy hh-mm-ss");

    EXPECT_STREQ(now.c_str(), latenow.c_str());
}

#include <optional>

TEST(DateTime, Exception)
{
    using namespace fl::utils;

    std::string now = DateTime::Now().ToString("dafsd   4252  2s");
    std::string latenow = DateTime::Now().ToString("strange text");
}