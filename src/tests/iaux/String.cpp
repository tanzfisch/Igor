#include <gtest/gtest.h>

#include <iaux/iaux.h>
#include <iaux/data/iaString.h>
using namespace iaux;

TEST(StringTests, Initial)
{
    iaString string;

    EXPECT_EQ(string.isEmpty(), true);
    EXPECT_EQ(string.getLength(), 0);
}

TEST(StringTests, SingleChar)
{
    iaString string('!');

    EXPECT_EQ(string.isEmpty(), false);
    EXPECT_EQ(string.getLength(), 1);
    EXPECT_EQ(string, '!');
}

TEST(StringTests, SingleWChar)
{
    iaString string(L'!');

    EXPECT_EQ(string.isEmpty(), false);
    EXPECT_EQ(string.getLength(), 1);
    EXPECT_EQ(string, L'!');
}

TEST(StringTests, Assignment)
{
    iaString string(L"FooBar");
    iaString string2 = string;
    iaString string3(string);

    EXPECT_EQ(string.isEmpty(), false);
    EXPECT_EQ(string.getLength(), 6);
    EXPECT_EQ(string, L"FooBar");
    EXPECT_EQ(string2, L"FooBar");
    EXPECT_EQ(string3, L"FooBar");
    EXPECT_EQ(string2, string);
}

TEST(StringTests, ToLower)
{
    iaString string(L"FooBar!124");
    string.toLower();

    EXPECT_EQ(string.isEmpty(), false);
    EXPECT_EQ(string.getLength(), 10);
    EXPECT_EQ(string, L"foobar!124");
}

TEST(StringTests, ToUpper)
{
    iaString string(L"FooBar!124");
    string.toUpper();

    EXPECT_EQ(string.isEmpty(), false);
    EXPECT_EQ(string.getLength(), 10);
    EXPECT_EQ(string, L"FOOBAR!124");
}

TEST(StringTests, getData)
{
    iaString string(L"FooBar!124こん");
    std::wstring value2(string.getData());
    std::wstring value1 = L"FooBar!124こん";

    EXPECT_EQ(value1, value2);
}

TEST(StringTests, IndexOperator)
{
    iaString string(L"FooBar!124こん");

    string[2] = 'z';
    string[5] = 'Z';

    EXPECT_EQ(string, L"FozBaZ!124こん");
    EXPECT_EQ(string[10], L'こ');
}

TEST(StringTests, SizeOperator)
{
    iaString string1(L"abc");
    iaString string2(L"def");

    EXPECT_EQ(string1 < string2, true);
    EXPECT_EQ(string1 > string2, false);
}

TEST(StringTests, FooBar)
{
    iaString string("FooBar");

    EXPECT_EQ(string.isEmpty(), false);
    EXPECT_EQ(string.getLength(), 6);
    EXPECT_EQ(string, "FooBar");
}

TEST(StringTests, ToFloat)
{
    EXPECT_EQ(iaString::toFloat(iaString(L"1.234")), 1.234);
    EXPECT_EQ(iaString::toFloat(iaString(L"-11.234")), -11.234);
}

TEST(StringTests, ToFloatFail)
{
    startup(); // since we are producing error messages we need to init the console
    EXPECT_EQ(iaString::toFloat(iaString(L"-1.1.234")), 0.0);
    EXPECT_EQ(iaString::toFloat(iaString(L"abc")), 0.0);
    shutdown();
}

TEST(StringTests, ToInt)
{
    EXPECT_EQ(iaString::toInt(iaString(L"100")), 100);
    EXPECT_EQ(iaString::toInt(iaString(L"-344")), -344);
}

TEST(StringTests, ToIntFail)
{
    startup(); // since we are producing error messages we need to init the console
    EXPECT_EQ(iaString::toInt(iaString(L"-3a44")), 0);
    EXPECT_EQ(iaString::toInt(iaString(L"-3.44")), 0);
    shutdown();
}

TEST(StringTests, ToString)
{
    EXPECT_EQ(iaString::toString(100), L"100");
    EXPECT_EQ(iaString::toString(100, 2), L"1100100");
    EXPECT_EQ(iaString::toString(100, 8), L"144");
    EXPECT_EQ(iaString::toString(100, 16), L"64");
    EXPECT_EQ(iaString::toString(-100), L"-100");
    EXPECT_EQ(iaString::toString(-10.23), L"-10.2300");
    EXPECT_EQ(iaString::toString(-10.234567,3), L"-10.234");
}

TEST(StringTests, Match)
{
    EXPECT_EQ(iaString::matchRegex("FooBar", ""), false);
    EXPECT_EQ(iaString::matchRegex("FooBar", "FooBar"), true);
    EXPECT_EQ(iaString::matchRegex("FooBar", "BarFoo"), false);

    EXPECT_EQ(iaString::matchRegex("FooBar", "(Foo)(.*)"), true);

    EXPECT_EQ(iaString::matchRegex("FooBar.ompf.back", "(.*)(.ompf.)(.*)"), true);
    EXPECT_EQ(iaString::matchRegex("FooBar.ompf.back", "(.*)(.ompf)"), false);
    EXPECT_EQ(iaString::matchRegex("FooBar.ompf", "(.*)(.ompf)"), true);
    EXPECT_EQ(iaString::matchRegex("crate.ompf", "(.*)(rate.ompf)"), true);
    EXPECT_EQ(iaString::matchRegex("igor/cat.ompf", "(.*)(rate.ompf)"), false);
    EXPECT_EQ(iaString::matchRegex("cat.ompf", "(cat.)(.*)"), true);
}

TEST(StringTests, Search)
{
    std::vector<iaString> matches;

    iaString::searchRegex(L"foo foobar frubar fuo bar", L"foo", matches);

    EXPECT_EQ(matches.size(), 3);
}

TEST(StringTests, Replace)
{
    iaString dst;
    iaString::replaceRegex("This is foobar.", "foobar", "great", dst);
    EXPECT_EQ(dst == "This is great.", true);
}

TEST(StringTests, UTF8Trivial)
{
    iaString string(L"abc123()");

    EXPECT_EQ(string.getUTF8Size(), 8);
    EXPECT_EQ(string.getLength(), 8);

    char utf8[8];
    int result = string.getUTF8(utf8, 8);
    EXPECT_EQ(result, 8);

    iaString string2;
    string2.setUTF8(utf8, 8);

    EXPECT_EQ(std::wstring(string.getData()), std::wstring(string2.getData()));
    EXPECT_EQ(string.getLength(), 8);
}

// we need to switch to full UTF-8 support so we don't have to deal with the wchar differences on windows and linux
/*TEST(StringTests, UTF8NonTrivial)
{
    iaString string(L"FooBar!124こん");

    EXPECT_EQ(string.getUTF8Size(), 16);
    EXPECT_EQ(string.getLength(), 12);

    char utf8[16];
    int result = string.getUTF8(utf8, 16);
    EXPECT_EQ(result, 16);

    iaString string2;
    string2.setUTF8(utf8, 16);

    EXPECT_EQ(std::wstring(string.getData()), std::wstring(string2.getData()));
    EXPECT_EQ(string.getLength(), 12);
}*/