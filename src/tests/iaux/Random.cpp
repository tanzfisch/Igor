#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/math/iaRandomNumberGenerator.h>
using namespace iaux;

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

IAUX_TEST(RandomTests, GetNextRangeExponentialIncrease)
{
    iaRandomNumberGenerator rand;
    rand.setSeed(1234);

    int counters[6];
    for(int i=0;i<6;++i)
    {
        counters[i] = 0;
    }

    for (int i = 0; i < 1000000; ++i)
    {
        counters[rand.getNextRangeExponentialIncrease(5, 10, 0.6) - 5]++;
    }

    IAUX_EXPECT_GREATER_THEN(counters[5], counters[4]);
    IAUX_EXPECT_GREATER_THEN(counters[4], counters[3]);
    IAUX_EXPECT_GREATER_THEN(counters[3], counters[2]);
    IAUX_EXPECT_GREATER_THEN(counters[2], counters[1]);
    IAUX_EXPECT_GREATER_THEN(counters[1], counters[0]);
}

IAUX_TEST(RandomTests, getNextRangeExponentialDecrease)
{
    iaRandomNumberGenerator rand;
    rand.setSeed(1234);

    int counters[6];
    for(int i=0;i<6;++i)
    {
        counters[i] = 0;
    }

    for (int i = 0; i < 1000000; ++i)
    {
        counters[rand.getNextRangeExponentialDecrease(5, 10, 0.6) - 5]++;
    }

    IAUX_EXPECT_LESS_THEN(counters[5], counters[4]);
    IAUX_EXPECT_LESS_THEN(counters[4], counters[3]);
    IAUX_EXPECT_LESS_THEN(counters[3], counters[2]);
    IAUX_EXPECT_LESS_THEN(counters[2], counters[1]);
    IAUX_EXPECT_LESS_THEN(counters[1], counters[0]);
}