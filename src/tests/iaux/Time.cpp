#include <gtest/gtest.h>

#include <iaux/system/iaTime.h>
using namespace iaux;

TEST(TimeTests, Initial)
{
    iaTime time;
    EXPECT_EQ(time.getMicrosenconds(), 0);
    EXPECT_EQ(time.getMilliseconds(), 0.0);
    EXPECT_EQ(time.getSeconds(), 0.0);
}

TEST(TimeTests, InitValue)
{
    iaTime time(100);
    EXPECT_EQ(time.getMicrosenconds(), 100);
    EXPECT_EQ(time.getMilliseconds(), 100.0 / 1000.0);
    EXPECT_EQ(time.getSeconds(), 100.0 / 1000000.0);
}

TEST(TimeTests, FromSeconds)
{
    iaTime time = iaTime::fromSeconds(12.3);
    EXPECT_EQ(time.getMicrosenconds(), static_cast<uint64>(12.3 * 1000000));
    EXPECT_EQ(time.getMilliseconds(), 12.3 * 1000.0);
    EXPECT_EQ(time.getSeconds(), 12.3);
}

TEST(TimeTests, FromMilliseconds)
{
    iaTime time = iaTime::fromMilliseconds(1234);
    EXPECT_EQ(time.getMicrosenconds(), 1234 * 1000);
    EXPECT_EQ(time.getMilliseconds(), 1234);
    EXPECT_EQ(time.getSeconds(), 1234 / 1000.0);
}

TEST(TimeTests, Operators)
{
    iaTime time1(100);
    iaTime time2(324);

    time1 += time2;
    EXPECT_EQ(time1.getMicrosenconds(), 424);
    EXPECT_EQ(time1.getMilliseconds(), 424.0 / 1000.0);
    EXPECT_EQ(time1.getSeconds(), 424.0 / 1000000.0);

    iaTime time3(1000);
    time1 -= time3;
    EXPECT_EQ(time1.getMicrosenconds(), -576);

    time2 = time1 + time3;
    EXPECT_EQ(time2.getMicrosenconds(), 424);

    time1 = time2 - time3;
    EXPECT_EQ(time1.getMicrosenconds(), -576);

    time1 *= 2;
    EXPECT_EQ(time1.getMicrosenconds(), -1152);

    time1 = time3 * 0.5;
    EXPECT_EQ(time1.getMicrosenconds(), 500);
}

TEST(TimeTests, Quality)
{
    iaTime time1(100);
    iaTime time2(324);

    EXPECT_EQ(time1 != time2, true);
    EXPECT_EQ(time1 == time2, false);
}

TEST(TimeTests, ComapareSize)
{
    iaTime time1(100);
    iaTime time2(324);
    iaTime time3(time2);

    EXPECT_EQ(time1 < time2, true);
    EXPECT_EQ(time1 > time2, false);
    EXPECT_EQ(time1 <= time2, true);
    EXPECT_EQ(time1 >= time2, false);
    EXPECT_EQ(time2 >= time3, true);
    EXPECT_EQ(time2 <= time3, true);
}