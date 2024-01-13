#include "fl/web/HttpQuery.hpp"
using namespace Forward::Web;

#include <gtest/gtest.h>

TEST(HttpQueryTest, ValidQuery) 
{
    std::string queryStr = "p=value1";
    HttpQuery query(queryStr);

    EXPECT_TRUE(query.IsValid());
    EXPECT_TRUE(query.HasKey("p"));

    EXPECT_TRUE(query.Value("p") == "value1");

    EXPECT_TRUE(query.Arg("p").GetName() == "p");
    EXPECT_TRUE(query.Arg("p").GetData() == "value1");
}

TEST(HttpQueryTest, MultipleQuery) 
{
    HttpQuery query("param1=value1&p=v");

    EXPECT_TRUE(query.IsValid());

    EXPECT_TRUE(query.HasKey("param1"));
    EXPECT_TRUE(query.HasKey("p"));

    EXPECT_TRUE(query.Value("param1") == "value1");
    EXPECT_TRUE(query.Value("p") == "v");

    EXPECT_TRUE(query.Arg("param1").GetData() == "value1");
    EXPECT_TRUE(query.Arg("p").GetData() == "v");
}