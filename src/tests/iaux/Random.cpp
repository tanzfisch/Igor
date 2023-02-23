#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/math/iaRandomNumberGenerator.h>
using namespace iaux;

IAUX_TEST(RandomTests, GetNext)
{
    iaRandomNumberGenerator rand;
    rand.setSeed(1234);

#define COUNT 100
    uint32 numbers[COUNT];

    for (int i = 0; i < COUNT; ++i)
    {
        numbers[i] = 0;
    }

    for (int i = 0; i < 10000000; ++i)
    {
        numbers[rand.getNext() % COUNT]++;
    }

    for (int i = 1; i < COUNT; ++i)
    {
        IAUX_EXPECT_NEAR(numbers[0], numbers[i], 800);
    }
}

IAUX_TEST(RandomTests, GetNextFloat)
{
    iaRandomNumberGenerator rand;
    rand.setSeed(1234);

    for (int i = 0; i < 1000000; ++i)
    {
        float64 value = rand.getNextFloat();
        IAUX_EXPECT_TRUE(value >= 0.0 && value <= 1.0);
    }
}

IAUX_TEST(RandomTests, GetNextFloatRange)
{
    iaRandomNumberGenerator rand;
    rand.setSeed(1234);

    for (int i = 0; i < 1000000; ++i)
    {
        float64 value = rand.getNextFloatRange(-10.0, -5.0);
        IAUX_EXPECT_TRUE(value >= -10.0 && value <= -5.0);
    }

    for (int i = 0; i < 1000000; ++i)
    {
        float64 value = rand.getNextFloatRange(5.0, 10.0);
        IAUX_EXPECT_TRUE(value >= 5.0 && value <= 10.0);
    }

    for (int i = 0; i < 1000000; ++i)
    {
        float64 value = rand.getNextFloatRange(-5.0, 10.0);
        IAUX_EXPECT_TRUE(value >= -5.0 && value <= 10.0);
    }
}

IAUX_TEST(RandomTests, GetNextRange1)
{
    iaRandomNumberGenerator rand;
    rand.setSeed(1234);

    for (int i = 0; i < 1000000; ++i)
    {
        int64 value = rand.getNextRange(-10, -5);
        IAUX_EXPECT_TRUE(value >= -10 && value <= -5);
    }

    for (int i = 0; i < 1000000; ++i)
    {
        int64 value = rand.getNextRange(5, 10);
        IAUX_EXPECT_TRUE(value >= 5 && value <= 10);
    }

    for (int i = 0; i < 1000000; ++i)
    {
        int64 value = rand.getNextRange(-5, 10);
        IAUX_EXPECT_TRUE(value >= -5 && value <= 10);
    }
}

IAUX_TEST(RandomTests, GetNextRange2)
{
    iaRandomNumberGenerator rand;
    rand.setSeed(1234);

    for (int i = 0; i < 1000000; ++i)
    {
        int64 value = rand.getNextRange(10);
        IAUX_EXPECT_TRUE(value >= 0 && value <= 10);
    }
}