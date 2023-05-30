#include "fl/utils/MimeType.hpp"

#include <gtest/gtest.h>

TEST(MimeType, IsExtension)
{
    using namespace fl::utils;

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
    using namespace fl::utils;

    EXPECT_TRUE(MimeType::FromString("txt").IsValid());
    EXPECT_TRUE(MimeType::FromString(".txt").IsValid());
    EXPECT_TRUE(MimeType::FromString("TEST.txt").IsValid());

    EXPECT_FALSE(MimeType::FromString("TESTtxt").IsValid());
    EXPECT_FALSE(MimeType::FromString("unknown").IsValid());
    EXPECT_FALSE(MimeType::FromString(".unknown").IsValid());
}

TEST(MimeType, RemoveExtension)
{
    using namespace fl::utils;

    EXPECT_TRUE(MimeType::RemoveExtension(".txt") == "");
    EXPECT_TRUE(MimeType::RemoveExtension("TEST.txt") == "TEST");
}