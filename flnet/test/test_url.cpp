#include "fl/net/http/HttpUrl.hpp"
using namespace fl;

#include <gtest/gtest.h>

TEST(HttpUrl, ParserTest)
{
    HttpUrl url("https://www.example.com/products/category/12345?page=2#section1");

    EXPECT_EQ(url.Target(), "https://www.example.com/products/category/12345");
    EXPECT_EQ(url.Section(), "section1");
    EXPECT_EQ(url.Value("page"), "2");
}