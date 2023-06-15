#include "fl/utils/MimeType.hpp"
using namespace fl;

#include <gtest/gtest.h>

TEST(MimeType, IsExtension)
{
    EXPECT_TRUE(MimeType::HasExtension("txt"));
    EXPECT_TRUE(MimeType::HasExtension("m4v"));
    EXPECT_TRUE(MimeType::HasExtension(".txt"));
    EXPECT_TRUE(MimeType::HasExtension("TEST.txt"));

    EXPECT_FALSE(MimeType::HasExtension("TESTtxt"));
    EXPECT_FALSE(MimeType::HasExtension("unknown"));
    EXPECT_FALSE(MimeType::HasExtension(".unknown"));
}

TEST(MimeType, FromString)
{
    auto txt1 = MimeType::FromString("txt");
    auto txt2 = MimeType::FromString(".txt");
    auto txt3 = MimeType::FromString("TEST.txt");
    auto szip = MimeType::FromString("TEST.7z");

    EXPECT_TRUE(txt1.IsValid());
    EXPECT_TRUE(txt2.IsValid());
    EXPECT_TRUE(txt3.IsValid());
    EXPECT_TRUE(szip.IsValid());

    EXPECT_EQ(txt1.GetType(), MimeType::Text);
    EXPECT_EQ(txt1.GetSubType(), MimeType::SubType::Plain);
    EXPECT_EQ(txt1.GetFormat(), "text/plain");

    auto bad = MimeType::FromString("TESTtxt");

    EXPECT_FALSE(bad.IsValid());
    EXPECT_EQ(bad.GetSubType(), MimeType::SubType::Unknown);
    EXPECT_EQ(bad.GetFormat(), "application/*");

    EXPECT_FALSE(MimeType::FromString("unknown").IsValid());
    EXPECT_FALSE(MimeType::FromString(".unknown").IsValid());
}

TEST(MimeType, RemoveExtension)
{
    EXPECT_TRUE(MimeType::RemoveExtension(".txt") == "");
    EXPECT_TRUE(MimeType::RemoveExtension("TEST.txt") == "TEST");
    EXPECT_TRUE(MimeType::RemoveExtension("TEST.7z") == "TEST");
}