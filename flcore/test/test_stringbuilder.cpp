#include "fl/core/StringBuilder.hpp"
using namespace Forward;

#include <gtest/gtest.h>

TEST(StringBuilder, ParsingFirst)
{
    StringArg arg1("arg1", "text with arguments"),
        arg2("arg2", "like this"),
        arg3("arg3", "don't like this");

    StringBuilder str_build("Some arg1: arg2, arg2, arg3.");

    str_build
        .ArgFirst(arg1)
        .ArgFirst(arg2)
        .ArgFirst(arg3);

    EXPECT_STREQ(str_build, "Some text with arguments: like this, like this, don't like this.");
}

TEST(StringBuilder, ParsingAll) 
{
    StringArg arg1("arg1", "text with arguments"), 
            arg2("arg2", "like this"), 
            arg3("arg3", "don't like this"); 

    StringBuilder str_build("Some arg1: arg2, arg2, arg3.");

    str_build
        .Arg(arg1)
        .Arg(arg2)
        .Arg(arg3);

    EXPECT_STREQ(str_build, "Some text with arguments: like this, like this, don't like this.");
}
