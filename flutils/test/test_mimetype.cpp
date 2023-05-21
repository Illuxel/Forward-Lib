#include "fl/utils/MimeType.hpp"

#include <gtest/gtest.h>

TEST(MimeType, IsExtension)
{
    using namespace fl::utils;

    EXPECT_TRUE(MimeType::IsExtension("txt"));
    EXPECT_TRUE(MimeType::IsExtension("m4v"));
    EXPECT_TRUE(MimeType::IsExtension(".txt"));
    EXPECT_TRUE(MimeType::IsExtension("TEST.txt"));

    EXPECT_FALSE(MimeType::IsExtension("TESTtxt"));
    EXPECT_FALSE(MimeType::IsExtension("unknown"));
    EXPECT_FALSE(MimeType::IsExtension(".unknown"));
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