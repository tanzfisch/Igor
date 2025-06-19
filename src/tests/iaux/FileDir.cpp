#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/system/iaFile.h>
#include <iaux/system/iaPath.h>
using namespace iaux;

IAUX_TEST(FileDirTests, Initial)
{
    iaPath dir("");
    IAUX_EXPECT_EQUAL(dir.getAbsolutePath().isEmpty(), true);
}

#ifdef IGOR_LINUX
IAUX_TEST(FileDirTests, FixPathLinux)
{
    IAUX_EXPECT_EQUAL(iaPath::fixPath(L"/foo/bar"), "/foo/bar");
    IAUX_EXPECT_EQUAL(iaPath::fixPath(L"/foo/blue/../bar"), "/foo/bar");
    IAUX_EXPECT_EQUAL(iaPath::fixPath(L"/foo/blue/../bar.txt"), "/foo/bar.txt");
}
#endif

#ifdef IGOR_WINDOWS
IAUX_TEST(FileDirTests, FixPathWindows)
{
    IAUX_EXPECT_EQUAL(iaPath::fixPath(L"c:\\foo\\bar"), "c:\\foo\\bar");
    IAUX_EXPECT_EQUAL(iaPath::fixPath(L"c:\\foo\\blue\\..\\bar"), "c:\\foo\\bar");
    IAUX_EXPECT_EQUAL(iaPath::fixPath(L"c:\\foo\\blue\\..\\bar.txt"), "c:\\foo\\bar.txt");
}
#endif