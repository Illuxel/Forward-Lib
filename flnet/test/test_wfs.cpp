#include "fl/net/http/WebFileSystem.hpp"
using namespace fl;

#include <gtest/gtest.h>

TEST(WebFileMeta, Data) {

    WebFileMeta wf = {
        "file_name",
        MimeType::FromString("txt"),
        "folder_name",
        "C:\\Test"
    };

    EXPECT_EQ(wf.File(), "file_name.txt");
    EXPECT_EQ(wf.FullPath(), "C:\\Test/file_name.txt");

    EXPECT_EQ(wf.TargetName(), "/file_name.txt");
    EXPECT_EQ(wf.TargetName(false), "/file_name");
    EXPECT_EQ(wf.TargetPath(), "/folder_name/file_name.txt");
}

TEST(WebFileSystem, TargetFileSearch) {


}