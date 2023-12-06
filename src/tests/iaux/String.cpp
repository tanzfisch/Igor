#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/iaux.h>
#include <iaux/data/iaString.h>
using namespace iaux;

#include <unordered_map>

IAUX_TEST(StringTests, Initial)
{
    iaString string;

    IAUX_EXPECT_EQUAL(string.isEmpty(), true);
    IAUX_EXPECT_EQUAL(string.getLength(), 0);
}

IAUX_TEST(StringTests, SingleChar)
{
    iaString string('!');

    IAUX_EXPECT_EQUAL(string.isEmpty(), false);
    IAUX_EXPECT_EQUAL(string.getLength(), 1);
    IAUX_EXPECT_EQUAL(string, '!');
}

IAUX_TEST(StringTests, SingleWChar)
{
    iaString string(L'!');

    IAUX_EXPECT_EQUAL(string.isEmpty(), false);
    IAUX_EXPECT_EQUAL(string.getLength(), 1);
    IAUX_EXPECT_EQUAL(string, L'!');
}

IAUX_TEST(StringTests, Assignment)
{
    iaString string(L"FooBar");
    iaString string2 = string;
    iaString string3(string);

    IAUX_EXPECT_EQUAL(string.isEmpty(), false);
    IAUX_EXPECT_EQUAL(string.getLength(), 6);
    IAUX_EXPECT_EQUAL(string, L"FooBar");
    IAUX_EXPECT_EQUAL(string2, L"FooBar");
    IAUX_EXPECT_EQUAL(string3, L"FooBar");
    IAUX_EXPECT_EQUAL(string2, string);
}

IAUX_TEST(StringTests, ToLower)
{
    iaString string(L"FooBar!124");
    string.toLower();

    IAUX_EXPECT_EQUAL(string.isEmpty(), false);
    IAUX_EXPECT_EQUAL(string.getLength(), 10);
    IAUX_EXPECT_EQUAL(string, L"foobar!124");
}

IAUX_TEST(StringTests, ToUpper)
{
    iaString string(L"FooBar!124");
    string.toUpper();

    IAUX_EXPECT_EQUAL(string.isEmpty(), false);
    IAUX_EXPECT_EQUAL(string.getLength(), 10);
    IAUX_EXPECT_EQUAL(string, L"FOOBAR!124");
}

IAUX_TEST(StringTests, getData)
{
    iaString string(L"FooBar!124こん");
    std::wstring value2(string.getData());
    std::wstring value1 = L"FooBar!124こん";

    IAUX_EXPECT_EQUAL(value1, value2);
}

IAUX_TEST(StringTests, IndexOperator)
{
    iaString string(L"FooBar!124こん");

    string[2] = 'z';
    string[5] = 'Z';

    IAUX_EXPECT_EQUAL(string, L"FozBaZ!124こん");
    IAUX_EXPECT_EQUAL(string[10], L'こ');
}

IAUX_TEST(StringTests, SizeOperator)
{
    iaString string1(L"abc");
    iaString string2(L"def");

    IAUX_EXPECT_TRUE(string1 < string2);
    IAUX_EXPECT_FALSE(string1 > string2);
}

IAUX_TEST(StringTests, FooBar)
{
    iaString string("FooBar");

    IAUX_EXPECT_EQUAL(string.isEmpty(), false);
    IAUX_EXPECT_EQUAL(string.getLength(), 6);
    IAUX_EXPECT_EQUAL(string, "FooBar");
}

IAUX_TEST(StringTests, ToFloat)
{
    IAUX_EXPECT_EQUAL(iaString::toFloat(iaString(L"1.234")), 1.234);
    IAUX_EXPECT_EQUAL(iaString::toFloat(iaString(L"-11.234")), -11.234);
}

IAUX_TEST(StringTests, ToInt)
{
    IAUX_EXPECT_EQUAL(iaString::toInt(L"100"), 100);
    IAUX_EXPECT_EQUAL(iaString::toInt(L"-344"), -344);
}

IAUX_TEST(StringTests, ToUInt)
{
    IAUX_EXPECT_EQUAL(iaString::toUInt(iaString(L"100")), 100);
    IAUX_EXPECT_EQUAL(iaString::toUInt(iaString(L"00344")), 344);

    IAUX_EXPECT_EQUAL(iaString::toUInt(L"0110", 2), 6);
    IAUX_EXPECT_EQUAL(iaString::toUInt(L"0144", 8), 100);

    IAUX_EXPECT_EQUAL(iaString::toUInt(L"9E78", 16), 40568);
    IAUX_EXPECT_EQUAL(iaString::toUInt(L"00FFff", 16), 65535);
    IAUX_EXPECT_EQUAL(iaString::toUInt(L"e75297ed09818a4c", 16), 16668552215174154828ULL);
    
}

IAUX_TEST(StringTests, ToString)
{
    IAUX_EXPECT_EQUAL(iaString::toString(100), L"100");
    IAUX_EXPECT_EQUAL(iaString::toString(100, 2), L"1100100");
    IAUX_EXPECT_EQUAL(iaString::toString(100, 8), L"144");
    IAUX_EXPECT_EQUAL(iaString::toString(100, 16), L"64");
    IAUX_EXPECT_EQUAL(iaString::toString(-100), L"-100");
    IAUX_EXPECT_EQUAL(iaString::toString(-10.23), L"-10.2300");
    IAUX_EXPECT_EQUAL(iaString::toString(-10.234567, 3), L"-10.234");
}

IAUX_TEST(StringTests, Match)
{
    IAUX_EXPECT_EQUAL(iaString::matchRegex("FooBar", ""), false);
    IAUX_EXPECT_EQUAL(iaString::matchRegex("FooBar", "FooBar"), true);
    IAUX_EXPECT_EQUAL(iaString::matchRegex("FooBar", "BarFoo"), false);

    IAUX_EXPECT_EQUAL(iaString::matchRegex("FooBar", "(Foo)(.*)"), true);

    IAUX_EXPECT_EQUAL(iaString::matchRegex("FooBar.ompf.back", "(.*)(.ompf.)(.*)"), true);
    IAUX_EXPECT_EQUAL(iaString::matchRegex("FooBar.ompf.back", "(.*)(.ompf)"), false);
    IAUX_EXPECT_EQUAL(iaString::matchRegex("FooBar.ompf", "(.*)(.ompf)"), true);
    IAUX_EXPECT_EQUAL(iaString::matchRegex("crate.ompf", "(.*)(rate.ompf)"), true);
    IAUX_EXPECT_EQUAL(iaString::matchRegex("igor/cat.ompf", "(.*)(rate.ompf)"), false);
    IAUX_EXPECT_EQUAL(iaString::matchRegex("cat.ompf", "(cat.)(.*)"), true);
}

IAUX_TEST(StringTests, Search)
{
    std::vector<iaString> matches;
    iaString::searchRegex(L"foo foobar frubar fuo bar", L"foo", matches);
    IAUX_EXPECT_EQUAL(matches.size(), 2);
}

IAUX_TEST(StringTests, Replace)
{
    iaString dst;
    iaString::replaceRegex("This is foobar.", "foobar", "great", dst);
    IAUX_EXPECT_EQUAL(dst, L"This is great.");
}

IAUX_TEST(StringTests, STDContainer)
{
    std::map<iaString, iaString> map;

    map["foo1"] = "bar1";
    map["foo2"] = "bar2";
    map["foo3"] = "bar3";
    map["foo4"] = "bar4";

    IAUX_EXPECT_EQUAL(map["foo1"], "bar1");
    IAUX_EXPECT_EQUAL(map["foo2"], "bar2");
    IAUX_EXPECT_EQUAL(map["foo3"], "bar3");
    IAUX_EXPECT_EQUAL(map["foo4"], "bar4");

    std::unordered_map<iaString, iaString> unorderedMap;

    unorderedMap["foo1"] = "bar1";
    unorderedMap["foo2"] = "bar2";
    unorderedMap["foo3"] = "bar3";
    unorderedMap["foo4"] = "bar4";

    IAUX_EXPECT_EQUAL(unorderedMap["foo1"], "bar1");
    IAUX_EXPECT_EQUAL(unorderedMap["foo2"], "bar2");
    IAUX_EXPECT_EQUAL(unorderedMap["foo3"], "bar3");
    IAUX_EXPECT_EQUAL(unorderedMap["foo4"], "bar4");
}

IAUX_TEST(StringTests, UTF8Trivial)
{
    iaString string(L"abc123()");

    IAUX_EXPECT_EQUAL(string.getUTF8Size(), 8);
    IAUX_EXPECT_EQUAL(string.getLength(), 8);

    char utf8[8];
    int result = string.getUTF8(utf8, 8);
    IAUX_EXPECT_EQUAL(result, 8);

    iaString string2;
    string2.setUTF8(utf8, 8);

    IAUX_EXPECT_EQUAL(std::wstring(string.getData()), std::wstring(string2.getData()));
    IAUX_EXPECT_EQUAL(string.getLength(), 8);
}

// we need to switch to full UTF-8 support so we don't have to deal with the wchar differences on windows and linux
/*IAUX_TEST(StringTests, UTF8NonTrivial)
{
    iaString string(L"FooBar!124こん");

    IAUX_EXPECT_EQUAL(string.getUTF8Size(), 16);
    IAUX_EXPECT_EQUAL(string.getLength(), 12);

    char utf8[16];
    int result = string.getUTF8(utf8, 16);
    IAUX_EXPECT_EQUAL(result, 16);

    iaString string2;
    string2.setUTF8(utf8, 16);

    IAUX_EXPECT_EQUAL(std::wstring(string.getData()), std::wstring(string2.getData()));
    IAUX_EXPECT_EQUAL(string.getLength(), 12);
}*/

IAUX_TEST(StringTests, WildcardsToRegEx)
{
    iaString string = "*";

    IAUX_EXPECT_EQUAL(iaString::wildcardToRegex(string), "foo");
}