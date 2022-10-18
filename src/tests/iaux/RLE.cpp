#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/data/iaRLE.h>
using namespace iaux;

typedef iaRLE<uint8, uint8> iaRLE88;

IAUX_TEST(RLETests, Initial)
{
    iaRLE88 data(10);
    iaRLE88 data2;

    IAUX_EXPECT_EQ(data.getSize(), 10);
    IAUX_EXPECT_EQ(data2.getSize(), 0);
}

IAUX_TEST(RLETests, SetSize)
{
    iaRLE88 data(5);

    IAUX_EXPECT_EQ(data.getSize(), 5);

    data.setSize(10);

    IAUX_EXPECT_EQ(data.getSize(), 10);
}

IAUX_TEST(RLETests, SetValue)
{
    iaRLE88 data(5);

    data.setValue(0, 1);
    data.setValue(1, 2);
    data.setValue(2, 3);
    data.setValue(3, 4);
    data.setValue(4, 5);

    IAUX_EXPECT_EQ(data.getSize(), 5);
    IAUX_EXPECT_EQ(data.getValue(0), 1);
    IAUX_EXPECT_EQ(data.getValue(1), 2);
    IAUX_EXPECT_EQ(data.getValue(2), 3);
    IAUX_EXPECT_EQ(data.getValue(3), 4);
    IAUX_EXPECT_EQ(data.getValue(4), 5);
}

IAUX_TEST(RLETests, Clear)
{
    iaRLE88 data(5);

    data.setValue(0, 1);
    data.setValue(1, 2);
    data.setValue(2, 3);
    data.setValue(3, 4);
    data.setValue(4, 5);

    IAUX_EXPECT_EQ(data.getSize(), 5);
    IAUX_EXPECT_EQ(data.getValue(0), 1);
    IAUX_EXPECT_EQ(data.getValue(1), 2);
    IAUX_EXPECT_EQ(data.getValue(2), 3);
    IAUX_EXPECT_EQ(data.getValue(3), 4);
    IAUX_EXPECT_EQ(data.getValue(4), 5);

    data.clear(15);

    IAUX_EXPECT_EQ(data.getSize(), 5);
    IAUX_EXPECT_EQ(data.getValue(0), 15);
    IAUX_EXPECT_EQ(data.getValue(1), 15);
    IAUX_EXPECT_EQ(data.getValue(2), 15);
    IAUX_EXPECT_EQ(data.getValue(3), 15);
    IAUX_EXPECT_EQ(data.getValue(4), 15);
}

IAUX_TEST(RLETests, SetValueReverseOrder)
{
    iaRLE88 data(5);

    data.setValue(4, 5);
    data.setValue(3, 4);
    data.setValue(2, 3);
    data.setValue(1, 2);
    data.setValue(0, 1);

    IAUX_EXPECT_EQ(data.getSize(), 5);
    IAUX_EXPECT_EQ(data.getValue(0), 1);
    IAUX_EXPECT_EQ(data.getValue(1), 2);
    IAUX_EXPECT_EQ(data.getValue(2), 3);
    IAUX_EXPECT_EQ(data.getValue(3), 4);
    IAUX_EXPECT_EQ(data.getValue(4), 5);
}

IAUX_TEST(RLETests, SetValueMixedOrder)
{
    iaRLE88 data(5);

    data.setValue(2, 3);
    data.setValue(4, 5);
    data.setValue(1, 2);
    data.setValue(3, 4);
    data.setValue(0, 1);

    IAUX_EXPECT_EQ(data.getSize(), 5);
    IAUX_EXPECT_EQ(data.getValue(0), 1);
    IAUX_EXPECT_EQ(data.getValue(1), 2);
    IAUX_EXPECT_EQ(data.getValue(2), 3);
    IAUX_EXPECT_EQ(data.getValue(3), 4);
    IAUX_EXPECT_EQ(data.getValue(4), 5);
}

IAUX_TEST(RLETests, SetValues)
{
    iaRLE88 data(5);

    data.setValue(0, 5, 42);

    IAUX_EXPECT_EQ(data.getSize(), 5);
    IAUX_EXPECT_EQ(data.getValue(0), 42);
    IAUX_EXPECT_EQ(data.getValue(1), 42);
    IAUX_EXPECT_EQ(data.getValue(2), 42);
    IAUX_EXPECT_EQ(data.getValue(3), 42);
    IAUX_EXPECT_EQ(data.getValue(4), 42);

    data.setValue(3, 2, 12);

    IAUX_EXPECT_EQ(data.getSize(), 5);
    IAUX_EXPECT_EQ(data.getValue(0), 42);
    IAUX_EXPECT_EQ(data.getValue(1), 42);
    IAUX_EXPECT_EQ(data.getValue(2), 42);
    IAUX_EXPECT_EQ(data.getValue(3), 12);
    IAUX_EXPECT_EQ(data.getValue(4), 12);

    data.setValue(1, 3, 72);

    IAUX_EXPECT_EQ(data.getSize(), 5);
    IAUX_EXPECT_EQ(data.getValue(0), 42);
    IAUX_EXPECT_EQ(data.getValue(1), 72);
    IAUX_EXPECT_EQ(data.getValue(2), 72);
    IAUX_EXPECT_EQ(data.getValue(3), 72);
    IAUX_EXPECT_EQ(data.getValue(4), 12);

    data.setValue(2, 3, 56);

    IAUX_EXPECT_EQ(data.getSize(), 5);
    IAUX_EXPECT_EQ(data.getValue(0), 42);
    IAUX_EXPECT_EQ(data.getValue(1), 72);
    IAUX_EXPECT_EQ(data.getValue(2), 56);
    IAUX_EXPECT_EQ(data.getValue(3), 56);
    IAUX_EXPECT_EQ(data.getValue(4), 56);
}
