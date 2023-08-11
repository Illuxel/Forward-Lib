#include "fl/net/http/HttpQuery.hpp"
using namespace Forward;

#include <gtest/gtest.h>

TEST(URICoding, Encode) {
    EXPECT_TRUE(UrlEncodeUtf8("Hello World") == "Hello%20World");
    EXPECT_TRUE(UrlEncodeUtf8("abc123!@#$%^&*()+-=") == "abc123%21%40%23%24%25%5E%26%2A%28%29%2B-%3D");
    EXPECT_TRUE(UrlEncodeUtf8("äöü") == "%C3%A4%C3%B6%C3%BC");
    EXPECT_TRUE(UrlEncodeUtf8("你好") == "%E4%BD%A0%E5%A5%BD");
}
TEST(URICoding, Decode) {

    EXPECT_TRUE("Hello World" == UrlDecodeUtf8("Hello%20World"));
    EXPECT_TRUE("abc123!@#$%^&*()+-=" == UrlDecodeUtf8("abc123%21%40%23%24%25%5E%26%2A%28%29%2B-%3D"));
    EXPECT_TRUE("äöü" == UrlDecodeUtf8("%C3%A4%C3%B6%C3%BC"));
    EXPECT_TRUE("你好" == UrlDecodeUtf8("%E4%BD%A0%E5%A5%BD"));
}

TEST(HttpQueryTest, ValidQuery) {
    std::string queryStr = "p=value1";
    HttpQuery query(queryStr);

    EXPECT_TRUE(query.IsValid());
    EXPECT_TRUE(query.HasKey("p"));

    EXPECT_TRUE(query.Value("p") == "value1");

    EXPECT_TRUE(query.Arg("p").GetName() == "p");
    EXPECT_TRUE(query.Arg("p").GetData() == "value1");
}

TEST(HttpQueryTest, MultipleQuery) {
    std::string queryStr = "param1=value1&p=v";
    HttpQuery query(queryStr);

    EXPECT_TRUE(query.IsValid());

    EXPECT_TRUE(query.HasKey("param1"));
    EXPECT_TRUE(query.HasKey("p"));

    EXPECT_TRUE(query.Value("param1") == "value1");
    EXPECT_TRUE(query.Value("p") == "v");

    EXPECT_TRUE(query.Arg("param1").GetData() == "value1");
    EXPECT_TRUE(query.Arg("p").GetData() == "v");
}