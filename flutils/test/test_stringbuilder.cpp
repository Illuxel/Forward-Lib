#include "fl/utils/StringBuilder.hpp"
using namespace fl::utils;

#include <gtest/gtest.h>

TEST(StringArgTest, ArgEqual) {
    StringArg arg1, arg2;

    arg1.SetArgSpecifier("test");

    EXPECT_FALSE(arg1 == arg2);

    arg2.SetArgSpecifier("test");

    EXPECT_TRUE(arg1 == arg2);
    EXPECT_TRUE(arg1 == "Hidden %test argument!");
}
TEST(StringArgTest, ArgCopy) {
    StringArg arg1;
    arg1.SetArgSpecifier("arg");

    StringArg copy(arg1);

    EXPECT_TRUE(copy == arg1);
}

TEST(StringBuilderTest, StringCreation) {

    StringArg arg1("arg1", "text with arguments"), arg2("arg2", "like this"), arg3("arg3", "don't like this"); 

    QByteArrayView arr = "Some %arg1: %arg2, %arg2, %arg3.";
    StringBuilder strb(arr.constData());

    strb.Arg(arg1).Arg(arg2).Arg(arg3);

    EXPECT_STREQ(strb, "Some text with arguments: like this, like this, don't like this.");
}

TEST(StringBuilderTest, StringBuilderCopying) {

    QByteArrayView arr = "Some %arg1: %arg2, %arg2, %arg3.";
    //StringBuilder strb = arr.constData();

}