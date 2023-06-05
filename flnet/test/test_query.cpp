#include "fl/net/http/HttpQuery.hpp"
using namespace fl;

#include <gtest/gtest.h>

TEST(HttpQueryTest, ValidQuery) {
    std::string queryStr = "p=value1";
    HttpQuery query(queryStr);

    EXPECT_FALSE(query.IsEmpty());
    EXPECT_TRUE(query.HasKey("p"));

    EXPECT_STREQ(query.Value("p").data(), "value1");

    EXPECT_STREQ(query.Arg("p").Name().data(), "p");
    EXPECT_STREQ(query.Arg("p").Data().data(), "value1");
}

TEST(HttpQueryTest, MultipleQuery) {
    std::string queryStr = "param1=value1&p=v";
    HttpQuery query(queryStr);


}