#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/system/iaFile.h>
#include <iaux/system/iaDirectory.h>
using namespace iaux;

IAUX_TEST(FileDirTests, Initial)
{
    iaFile file("");
    iaDirectory dir("");

    IAUX_EXPECT_EQUAL(file.getFullFileName().isEmpty(), true);
    IAUX_EXPECT_EQUAL(dir.getFullDirectoryName().isEmpty(), true);
}

#ifdef __IGOR_LINUX__
IAUX_TEST(FileDirTests, FixPathLinux)
{
    IAUX_EXPECT_EQUAL(iaDirectory::fixPath(L"/foo/bar", false), "/foo/bar");
    IAUX_EXPECT_EQUAL(iaDirectory::fixPath(L"/foo/blue/../bar", false), "/foo/bar");
    IAUX_EXPECT_EQUAL(iaDirectory::fixPath(L"/foo/blue/../bar.txt", true), "/foo/bar.txt");
}
#endif

#ifdef IGOR_WINDOWS
IAUX_TEST(FileDirTests, FixPathWindows)
{
    IAUX_EXPECT_EQUAL(iaDirectory::fixPath(L"c:\\foo\\bar", false), "c:\\foo\\bar");
    IAUX_EXPECT_EQUAL(iaDirectory::fixPath(L"c:\\foo\\blue\\..\\bar", false), "c:\\foo\\bar");
    IAUX_EXPECT_EQUAL(iaDirectory::fixPath(L"c:\\foo\\blue\\..\\bar.txt", true), "c:\\foo\\bar.txt");
}
#endif