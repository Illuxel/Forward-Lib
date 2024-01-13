#include "fl/web/HttpUrlParser.hpp"
using namespace Forward::Web;

#include <gtest/gtest.h>

TEST(Uri, Encode) 
{
    EXPECT_TRUE(HttpUrlParser::EncodeUtf8("Hello World") == "Hello+World");
    EXPECT_TRUE(HttpUrlParser::EncodeUtf8("abc123!@#$%^&*()+-=") == "abc123%21%40%23%24%25%5E%26%2A%28%29%2B-%3D");
    EXPECT_TRUE(HttpUrlParser::EncodeUtf8("äöü") == "%C3%A4%C3%B6%C3%BC");
    EXPECT_TRUE(HttpUrlParser::EncodeUtf8("你好") == "%E4%BD%A0%E5%A5%BD");
}
TEST(Uri, Decode) 
{
    EXPECT_TRUE("Hello World" == HttpUrlParser::DecodeUtf8("Hello+World"));
    EXPECT_TRUE("abc123!@#$%^&*()+-=" == HttpUrlParser::DecodeUtf8("abc123%21%40%23%24%25%5E%26%2A%28%29%2B-%3D"));
    EXPECT_TRUE("äöü" == HttpUrlParser::DecodeUtf8("%C3%A4%C3%B6%C3%BC"));
    EXPECT_TRUE("你好" == HttpUrlParser::DecodeUtf8("%E4%BD%A0%E5%A5%BD"));
}