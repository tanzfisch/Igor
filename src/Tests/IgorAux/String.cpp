#include <gtest/gtest.h>

#include <iaString.h>
using namespace IgorAux;

#include <iconv.h>

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
    iaString string(L"FooBar!124こん");
    string.toLower();

    EXPECT_EQ(string.isEmpty(), false);
    EXPECT_EQ(string.getLength(), 12);
    EXPECT_EQ(string, L"foobar!124こん");
}

TEST(StringTests, ToUpper)
{
    iaString string(L"FooBar!124こん");
    string.toUpper();

    EXPECT_EQ(string.isEmpty(), false);
    EXPECT_EQ(string.getLength(), 12);
    EXPECT_EQ(string, L"FOOBAR!124こん");
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

TEST(StringTests, WideCharacters)
{
    iaString string(L"¡Hola!こんにちは");

    EXPECT_EQ(string.isEmpty(), false);
    EXPECT_EQ(string.getLength(), 11);
    EXPECT_EQ(string, L"¡Hola!こんにちは");
}

TEST(StringTests, UTF8)
{
    iaString string(L"¡こんにちは!");

    EXPECT_EQ(string.getUTF8Size(), 19);
    EXPECT_EQ(string.getLength(), 7);

    char utf8[19];
    string.getUTF8(utf8, 19);

    string.setUTF8(utf8, 19);

    EXPECT_EQ(string.getUTF8Size(), 19);
    EXPECT_EQ(string.getLength(), 7);
    EXPECT_EQ(string, L"¡こんにちは!");
}
