#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/iaux.h>
#include <iaux/data/iaString.h>
using namespace iaux;

IAUX_TEST(StringTests, Initial)
{
    iaString string;

    IAUX_EXPECT_EQ(string.isEmpty(), true);
    IAUX_EXPECT_EQ(string.getLength(), 0);
}

IAUX_TEST(StringTests, SingleChar)
{
    iaString string('!');

    IAUX_EXPECT_EQ(string.isEmpty(), false);
    IAUX_EXPECT_EQ(string.getLength(), 1);
    IAUX_EXPECT_EQ(string, '!');
}

IAUX_TEST(StringTests, SingleWChar)
{
    iaString string(L'!');

    IAUX_EXPECT_EQ(string.isEmpty(), false);
    IAUX_EXPECT_EQ(string.getLength(), 1);
    IAUX_EXPECT_EQ(string, L'!');
}

IAUX_TEST(StringTests, Assignment)
{
    iaString string(L"FooBar");
    iaString string2 = string;
    iaString string3(string);

    IAUX_EXPECT_EQ(string.isEmpty(), false);
    IAUX_EXPECT_EQ(string.getLength(), 6);
    IAUX_EXPECT_EQ(string, L"FooBar");
    IAUX_EXPECT_EQ(string2, L"FooBar");
    IAUX_EXPECT_EQ(string3, L"FooBar");
    IAUX_EXPECT_EQ(string2, string);
}

IAUX_TEST(StringTests, ToLower)
{
    iaString string(L"FooBar!124");
    string.toLower();

    IAUX_EXPECT_EQ(string.isEmpty(), false);
    IAUX_EXPECT_EQ(string.getLength(), 10);
    IAUX_EXPECT_EQ(string, L"foobar!124");
}

IAUX_TEST(StringTests, ToUpper)
{
    iaString string(L"FooBar!124");
    string.toUpper();

    IAUX_EXPECT_EQ(string.isEmpty(), false);
    IAUX_EXPECT_EQ(string.getLength(), 10);
    IAUX_EXPECT_EQ(string, L"FOOBAR!124");
}

IAUX_TEST(StringTests, getData)
{
    iaString string(L"FooBar!124こん");
    std::wstring value2(string.getData());
    std::wstring value1 = L"FooBar!124こん";

    IAUX_EXPECT_EQ(value1, value2);
}

IAUX_TEST(StringTests, IndexOperator)
{
    iaString string(L"FooBar!124こん");

    string[2] = 'z';
    string[5] = 'Z';

    IAUX_EXPECT_EQ(string, L"FozBaZ!124こん");
    IAUX_EXPECT_EQ(string[10], L'こ');
}

IAUX_TEST(StringTests, SizeOperator)
{
    iaString string1(L"abc");
    iaString string2(L"def");

    IAUX_EXPECT_EQ(string1 < string2, true);
    IAUX_EXPECT_EQ(string1 > string2, false);
}

IAUX_TEST(StringTests, FooBar)
{
    iaString string("FooBar");

    IAUX_EXPECT_EQ(string.isEmpty(), false);
    IAUX_EXPECT_EQ(string.getLength(), 6);
    IAUX_EXPECT_EQ(string, "FooBar");
}

IAUX_TEST(StringTests, ToFloat)
{
    IAUX_EXPECT_EQ(iaString::toFloat(iaString(L"1.234")), 1.234);
    IAUX_EXPECT_EQ(iaString::toFloat(iaString(L"-11.234")), -11.234);
}

IAUX_TEST(StringTests, ToFloatFail)
{
    startup(); // since we are producing error messages we need to init the console
    IAUX_EXPECT_EQ(iaString::toFloat(iaString(L"-1.1.234")), 0.0);
    IAUX_EXPECT_EQ(iaString::toFloat(iaString(L"abc")), 0.0);
    shutdown();
}

IAUX_TEST(StringTests, ToInt)
{
    IAUX_EXPECT_EQ(iaString::toInt(iaString(L"100")), 100);
    IAUX_EXPECT_EQ(iaString::toInt(iaString(L"-344")), -344);
}

IAUX_TEST(StringTests, ToIntFail)
{
    startup(); // since we are producing error messages we need to init the console
    IAUX_EXPECT_EQ(iaString::toInt(iaString(L"-3a44")), 0);
    IAUX_EXPECT_EQ(iaString::toInt(iaString(L"-3.44")), 0);
    shutdown();
}

IAUX_TEST(StringTests, ToString)
{
    IAUX_EXPECT_EQ(iaString::toString(100), L"100");
    IAUX_EXPECT_EQ(iaString::toString(100, 2), L"1100100");
    IAUX_EXPECT_EQ(iaString::toString(100, 8), L"144");
    IAUX_EXPECT_EQ(iaString::toString(100, 16), L"64");
    IAUX_EXPECT_EQ(iaString::toString(-100), L"-100");
    IAUX_EXPECT_EQ(iaString::toString(-10.23), L"-10.2300");
    IAUX_EXPECT_EQ(iaString::toString(-10.234567,3), L"-10.234");
}

IAUX_TEST(StringTests, Match)
{
    IAUX_EXPECT_EQ(iaString::matchRegex("FooBar", ""), false);
    IAUX_EXPECT_EQ(iaString::matchRegex("FooBar", "FooBar"), true);
    IAUX_EXPECT_EQ(iaString::matchRegex("FooBar", "BarFoo"), false);

    IAUX_EXPECT_EQ(iaString::matchRegex("FooBar", "(Foo)(.*)"), true);

    IAUX_EXPECT_EQ(iaString::matchRegex("FooBar.ompf.back", "(.*)(.ompf.)(.*)"), true);
    IAUX_EXPECT_EQ(iaString::matchRegex("FooBar.ompf.back", "(.*)(.ompf)"), false);
    IAUX_EXPECT_EQ(iaString::matchRegex("FooBar.ompf", "(.*)(.ompf)"), true);
    IAUX_EXPECT_EQ(iaString::matchRegex("crate.ompf", "(.*)(rate.ompf)"), true);
    IAUX_EXPECT_EQ(iaString::matchRegex("igor/cat.ompf", "(.*)(rate.ompf)"), false);
    IAUX_EXPECT_EQ(iaString::matchRegex("cat.ompf", "(cat.)(.*)"), true);
}

IAUX_TEST(StringTests, Search)
{
    std::vector<iaString> matches;
    iaString::searchRegex(L"foo foobar frubar fuo bar", L"foo", matches);
    IAUX_EXPECT_EQ(matches.size(), 2);
}

IAUX_TEST(StringTests, Replace)
{
    iaString dst;
    iaString::replaceRegex("This is foobar.", "foobar", "great", dst);
    IAUX_EXPECT_EQ(dst == "This is great.", true);
}

IAUX_TEST(StringTests, UTF8Trivial)
{
    iaString string(L"abc123()");

    IAUX_EXPECT_EQ(string.getUTF8Size(), 8);
    IAUX_EXPECT_EQ(string.getLength(), 8);

    char utf8[8];
    int result = string.getUTF8(utf8, 8);
    IAUX_EXPECT_EQ(result, 8);

    iaString string2;
    string2.setUTF8(utf8, 8);

    IAUX_EXPECT_EQ(std::wstring(string.getData()), std::wstring(string2.getData()));
    IAUX_EXPECT_EQ(string.getLength(), 8);
}

// we need to switch to full UTF-8 support so we don't have to deal with the wchar differences on windows and linux
/*IAUX_TEST(StringTests, UTF8NonTrivial)
{
    iaString string(L"FooBar!124こん");

    IAUX_EXPECT_EQ(string.getUTF8Size(), 16);
    IAUX_EXPECT_EQ(string.getLength(), 12);

    char utf8[16];
    int result = string.getUTF8(utf8, 16);
    IAUX_EXPECT_EQ(result, 16);

    iaString string2;
    string2.setUTF8(utf8, 16);

    IAUX_EXPECT_EQ(std::wstring(string.getData()), std::wstring(string2.getData()));
    IAUX_EXPECT_EQ(string.getLength(), 12);
}*/