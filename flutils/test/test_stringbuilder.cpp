#include "fl/utils/StringBuilder.hpp"
#include <gtest/gtest.h>

TEST(StringArg, ArgEqual) 
{
    using namespace fl::utils;

    StringArg arg1("test"), arg2;

    EXPECT_FALSE(arg1 == arg2);

    arg2.SetName("test");

    EXPECT_TRUE(arg1 == arg2);
    EXPECT_TRUE(arg1 == "Hidden test argument!");
}
TEST(StringArg, ArgCopy) 
{
    using namespace fl::utils;

    StringArg arg1("arg");

    arg1.SetData("hello world");

    StringArg copy(arg1);

    EXPECT_TRUE(copy == arg1);
    EXPECT_TRUE(arg1.Data() == copy.Data());

    copy.SetName("NEW");

    EXPECT_FALSE(copy == arg1);
}

TEST(StringArgSpec, ArgEqual) 
{
    using namespace fl::utils;

    StringArg arg1("test", '%'), arg2('%');

    EXPECT_FALSE(arg1 == arg2);

    arg2.SetName("test");

    EXPECT_TRUE(arg1 == arg2);
    EXPECT_TRUE(arg1 == "Hidden %test argument!");
}
TEST(StringArgSpec, ArgCopy) 
{
    using namespace fl::utils;

    StringArg arg1("arg", '%');
    StringArg copy(arg1);

    EXPECT_TRUE(copy == arg1);
}

TEST(StringBuilder, StringCreation) 
{
    using namespace fl::utils;

    StringArg arg1("arg1", "text with arguments"), 
            arg2("arg2", "like this"), 
            arg3("arg3", "don't like this"); 

    StringBuilder strb("Some arg1: arg2, arg2, arg3.");

    strb.Arg(arg1).Arg(arg2).Arg(arg3);

    EXPECT_STREQ(strb, "Some text with arguments: like this, like this, don't like this.");
}

TEST(StringBuilderSpec, StringCreation) 
{
    using namespace fl::utils;

    StringArg arg1("arg1", "text with arguments", '%'), 
            arg2("arg2", "like this", '%'), 
            arg3("arg3", "don't like this", '%'); 

    StringBuilder strb("Some %arg1: %arg2, %arg2, %arg3.");

    strb.Arg(arg1).Arg(arg2).Arg(arg3);

    EXPECT_STREQ(strb, "Some text with arguments: like this, like this, don't like this.");
}