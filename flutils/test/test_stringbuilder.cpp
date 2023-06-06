#include "fl/utils/StringBuilder.hpp"
using namespace fl;

#include <gtest/gtest.h>

TEST(StringBuilder, StringCreationRef) 
{
    StringArg arg1("arg1", "text with arguments"), 
            arg2("arg2", "like this"), 
            arg3("arg3", "don't like this"); 

    StringBuilder strb("Some arg1: arg2, arg2, arg3.");

    strb.Arg(arg1).Arg(arg2).Arg(arg3);

    EXPECT_STREQ(strb, "Some text with arguments: like this, like this, don't like this.");
}

TEST(StringBuilderSpec, StringCreationRef) 
{
    StringArg arg1("arg1", "text with arguments", '%'), 
            arg2("arg2", "like this", '%'), 
            arg3("arg3", "don't like this", '%'); 

    StringBuilder strb("Some %arg1: %arg2, %arg2, %arg3.");

    strb.Arg(arg1).Arg(arg2).Arg(arg3);

    EXPECT_STREQ(strb, "Some text with arguments: like this, like this, don't like this.");
}