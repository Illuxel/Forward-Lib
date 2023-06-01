#include "fl/net/http/HttpUrl.hpp"
using namespace fl;

#include <gtest/gtest.h>

TEST(HttpUrl, ParserTest)
{
    HttpUrl url("https://www.example.com/path/to/resource?param1=value1&param2=value2#fragment");

    EXPECT_STREQ(url.Domain().data(), "www.example.com");
    EXPECT_STREQ(url.Target().data(), "/path/to/resource");
    EXPECT_STREQ(url.Section().data(), "fragment");

    EXPECT_STREQ(url.Value("param1").data(), "value1");
    EXPECT_STREQ(url.Value("param2").data(), "value2");
}

TEST(HttpUrl, BadParserTest)
{
    HttpUrl url("https:/www.example.com/path/to/resourceparam1=value1&param2=value2#fragment");

    EXPECT_FALSE(url.HasDomain());
    EXPECT_FALSE(url.HasTarget());
    EXPECT_FALSE(url.HasSection());

    EXPECT_FALSE(url.HasKey("param1"));
    EXPECT_FALSE(url.HasKey("param2"));

    EXPECT_FALSE(url.IsValueEmpty("param1"));
    EXPECT_FALSE(url.IsValueEmpty("param2"));
}