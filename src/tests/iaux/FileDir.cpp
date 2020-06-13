#include <gtest/gtest.h>

#include <iaux/system/iaFile.h>
#include <iaux/system/iaDirectory.h>
using namespace iaux;

TEST(FileDirTests, Initial)
{
    iaFile file("");
    iaDirectory dir("");

    EXPECT_EQ(file.getFullFileName().isEmpty(), true);
    EXPECT_EQ(dir.getFullDirectoryName().isEmpty(), true);
}

#ifdef __IGOR_LINUX__
TEST(FileDirTests, FixPathLinux)
{
    EXPECT_EQ(iaDirectory::fixPath(L"/foo/bar", false), "/foo/bar");
    EXPECT_EQ(iaDirectory::fixPath(L"/foo/blue/../bar", false), "/foo/bar");
    EXPECT_EQ(iaDirectory::fixPath(L"/foo/blue/../bar.txt", true), "/foo/bar.txt");
}
#endif

#ifdef __IGOR_WINDOWS__
TEST(FileDirTests, FixPathWindows)
{
    EXPECT_EQ(iaDirectory::fixPath(L"c:\\foo\\bar", false), "c:\\foo\\bar");
    EXPECT_EQ(iaDirectory::fixPath(L"c:\\foo\\blue\\..\\bar", false), "c:\\foo\\bar");
    EXPECT_EQ(iaDirectory::fixPath(L"c:\\foo\\blue\\..\\bar.txt", true), "c:\\foo\\bar.txt");
}
#endif