#include "fl/core/StringArg.hpp"
#include "fl/core/StringArgParser.hpp"
using namespace Forward;

#include <gtest/gtest.h>

TEST(StringArg, ArgEqual) 
{
    StringArg arg1("test"), arg2;

    EXPECT_FALSE(arg1 == arg2);

    arg2.SetName("test");

    EXPECT_TRUE(arg1 == arg2);
    EXPECT_TRUE(arg1 == "Hidden test argument!");
}
TEST(StringArg, ArgCopy) 
{
    StringArg arg1("arg");

    arg1.GetData() = "hello world";

    StringArg copy(arg1);

    EXPECT_TRUE(copy == arg1);
    EXPECT_TRUE(arg1.GetData() == copy.GetData());

    copy.SetName("NEW");

    EXPECT_FALSE(copy == arg1);
}

TEST(StringArg, MakeArg)
{
    std::string_view str = "key=arg_data";
    StringArg arg = StringArgParser::FromString(str, '=');

    EXPECT_TRUE(arg == "key");
    EXPECT_TRUE(arg.GetData() == "arg_data");
}