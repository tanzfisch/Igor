#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/system/iaTime.h>
using namespace iaux;

IAUX_TEST(TimeTests, Initial)
{
    iaTime time;
    IAUX_EXPECT_EQUAL(time.getMicroseconds(), 0);
    IAUX_EXPECT_EQUAL(time.getMilliseconds(), 0.0);
    IAUX_EXPECT_EQUAL(time.getSeconds(), 0.0);
}

IAUX_TEST(TimeTests, InitValue)
{
    iaTime time(100);
    IAUX_EXPECT_EQUAL(time.getMicroseconds(), 100);
    IAUX_EXPECT_EQUAL(time.getMilliseconds(), 100.0 / 1000.0);
    IAUX_EXPECT_EQUAL(time.getSeconds(), 100.0 / 1000000.0);
}

IAUX_TEST(TimeTests, FromSeconds)
{
    iaTime time = iaTime::fromSeconds(12.3);
    IAUX_EXPECT_EQUAL(time.getMicroseconds(), static_cast<uint64>(12.3 * 1000000));
    IAUX_EXPECT_EQUAL(time.getMilliseconds(), 12.3 * 1000.0);
    IAUX_EXPECT_EQUAL(time.getSeconds(), 12.3);
}

IAUX_TEST(TimeTests, FromMilliseconds)
{
    iaTime time = iaTime::fromMilliseconds(1234);
    IAUX_EXPECT_EQUAL(time.getMicroseconds(), 1234 * 1000);
    IAUX_EXPECT_EQUAL(time.getMilliseconds(), 1234);
    IAUX_EXPECT_EQUAL(time.getSeconds(), 1234 / 1000.0);
}

IAUX_TEST(TimeTests, Operators)
{
    iaTime time1(100);
    iaTime time2(324);

    time1 += time2;
    IAUX_EXPECT_EQUAL(time1.getMicroseconds(), 424);
    IAUX_EXPECT_EQUAL(time1.getMilliseconds(), 424.0 / 1000.0);
    IAUX_EXPECT_EQUAL(time1.getSeconds(), 424.0 / 1000000.0);

    iaTime time3(1000);
    time1 -= time3;
    IAUX_EXPECT_EQUAL(time1.getMicroseconds(), -576);

    time2 = time1 + time3;
    IAUX_EXPECT_EQUAL(time2.getMicroseconds(), 424);

    time1 = time2 - time3;
    IAUX_EXPECT_EQUAL(time1.getMicroseconds(), -576);

    time1 *= 2;
    IAUX_EXPECT_EQUAL(time1.getMicroseconds(), -1152);

    time1 = time3 * 0.5;
    IAUX_EXPECT_EQUAL(time1.getMicroseconds(), 500);
}

IAUX_TEST(TimeTests, Quality)
{
    iaTime time1(100);
    iaTime time2(324);

    IAUX_EXPECT_TRUE(time1 != time2);
    IAUX_EXPECT_FALSE(time1 == time2);
}

IAUX_TEST(TimeTests, ComapareSize)
{
    iaTime time1(100);
    iaTime time2(324);
    iaTime time3(time2);

    IAUX_EXPECT_TRUE(time1 < time2);
    IAUX_EXPECT_FALSE(time1 > time2);
    IAUX_EXPECT_TRUE(time1 <= time2);
    IAUX_EXPECT_FALSE(time1 >= time2);
    IAUX_EXPECT_TRUE(time2 >= time3);
    IAUX_EXPECT_TRUE(time2 <= time3);
}