#include "fl/utils/DateTime.hpp"
using namespace Forward;

#include <gtest/gtest.h>

TEST(DateTime, ToCFormat)
{
    std::string out = DateTime::ConvertToCFormat("DD-MM-YYYY hh-mm-ss");

    EXPECT_TRUE(out == "%d-%m-%Y %H-%M-%S");
}

TEST(DateTime, NowToString)
{
    std::string now = DateTime::Now().ToString("DD-MM-YYYY hh-mm-ss");

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::string late_now = DateTime::Now().ToString("DD-MM-YYYY hh-mm-ss");

    EXPECT_FALSE(now == late_now);
}

TEST(DateTime, CopyEq)
{
    auto now = DateTime::Now();
    auto copy = now;

    EXPECT_TRUE(now == copy);
}

TEST(DateTime, FailParse)
{
    std::string now = DateTime::Now().ToString("dafsd   4252  2s");
    std::string late_now = DateTime::Now().ToString("strange text");

    EXPECT_TRUE(now.empty());
    EXPECT_TRUE(late_now.empty());
}