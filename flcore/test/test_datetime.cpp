#include "fl/core/DateTime.hpp"
using namespace Forward;

#include <gtest/gtest.h>

TEST(DateTime, ToCFormatParse)
{
    std::string out = DateTime::ConvertToCFormat("DD-MM-YYYY hh:mm:ss");

    EXPECT_TRUE(out == "%d-%m-%Y %H:%M:%S");
}

TEST(DateTime, Copy)
{
    auto now = DateTime::Now();
    auto copy = now;

    EXPECT_TRUE(now.IsValid());
    EXPECT_TRUE(copy.IsValid());

    EXPECT_TRUE(now == copy);
}

TEST(DateTime, FromString)
{
    DateTime dtime1 = DateTime::FromString("16-12-2023 15:12:30");
    DateTime dtime2 = DateTime::FromString("1-1-2023 15:12:30");

    EXPECT_TRUE(dtime1.IsValid());
    EXPECT_TRUE(dtime2.IsValid());

    EXPECT_TRUE(dtime1.ToString("MM") == "12");
    EXPECT_TRUE(dtime2.ToString("YYYY") == "2023");
}

TEST(DateTime, FailParse)
{
    std::string now = DateTime::Now().ToString("dafsd   4252  2s");
    std::string late_now = DateTime::Now().ToString("strange text");

    EXPECT_TRUE(now == "dafsd   4252  2s");
    EXPECT_TRUE(late_now == "strange text");
}