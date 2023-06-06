#include "fl/utils/StringArg.hpp"
using namespace fl;

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

    arg1.SetData("hello world");

    StringArg copy(arg1);

    EXPECT_TRUE(copy == arg1);
    EXPECT_TRUE(arg1.Data() == copy.Data());

    copy.SetName("NEW");

    EXPECT_FALSE(copy == arg1);
}

TEST(StringArg, MakeArg) 
{
    std::string_view str = "arg_name=arg_data";
    auto arg = StringArg::FromString(str, '=');

    EXPECT_TRUE(arg == "arg_name");
    EXPECT_TRUE(arg.Data() == "arg_data");
}

TEST(StringArgSpecifier, ArgEqual) 
{
    StringArg arg1("test", '%'), arg2('%');

    EXPECT_FALSE(arg1 == arg2);

    arg2.SetName("test");

    EXPECT_TRUE(arg1 == arg2);
    EXPECT_TRUE(arg1 == "Hidden %test argument!");
}
TEST(StringArgSpecifier, ArgCopy) 
{
    StringArg arg1("arg", '%');
    StringArg copy(arg1);

    EXPECT_TRUE(copy == arg1);
}