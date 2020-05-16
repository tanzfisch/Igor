#include <gtest/gtest.h>

#include <iaString.h>
using namespace IgorAux;

TEST(StringTests, Initial)
{
    iaString string;

    EXPECT_EQ(string.isEmpty(), true);
    EXPECT_EQ(string.getLength(), 0);
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

    EXPECT_EQ(string.getUTF8Size(), 18);
    EXPECT_EQ(string.getLength(), 7);

    char utf8[18];
    string.getUTF8(utf8, 18);

    string.setUTF8(utf8, 18);

    EXPECT_EQ(string.getUTF8Size(), 18);
    EXPECT_EQ(string.getLength(), 7);
    EXPECT_EQ(string, L"¡こんにちは!");
}


TEST(StringTests, Assignment)
{
    iaString string(L"FooBar");

    iaString string2 = string;

    EXPECT_EQ(string.isEmpty(), false);
    EXPECT_EQ(string.getLength(), 6);
    EXPECT_EQ(string, L"FooBar");
    EXPECT_EQ(string2, L"FooBar");
}


