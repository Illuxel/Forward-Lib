#include "fl/net/http/WebFilesSystem.hpp"
using namespace fl;

#include <gtest/gtest.h>

TEST(WebFileMeta, Data) {

    WebFileMeta wf = {
        "file_name",
        "folder_name",
        MimeType::FromString("txt"),
        "C:\\Test"
    };

    EXPECT_EQ(wf.File(), "file_name.txt");
    EXPECT_EQ(wf.FullPath(), "C:\\Test/file_name.txt");

    EXPECT_EQ(wf.TargetName(), "/file_name.txt");
    EXPECT_EQ(wf.TargetName(false), "/file_name");
    EXPECT_EQ(wf.TargetPath(), "/folder_name/file_name.txt");
}