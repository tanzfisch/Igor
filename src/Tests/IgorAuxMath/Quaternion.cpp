#include "pch.h"
#include <iaQuaternion.h>
using namespace IgorAux;

TEST(QuaternionTests, Initial)
{
	iaQuaterniond quat;

	EXPECT_EQ(quat._x, 0);
	EXPECT_EQ(quat._y, 0);
	EXPECT_EQ(quat._z, 0);
	EXPECT_EQ(quat._w, 0);
}

TEST(QuaternionTests, Set)
{
	iaQuaterniond quat;
	quat.set(1, 2, 3, 4);

	EXPECT_EQ(quat._x, 1);
	EXPECT_EQ(quat._y, 2);
	EXPECT_EQ(quat._z, 3);
	EXPECT_EQ(quat._w, 4);
}

TEST(QuaternionTests, SetEuler)
{
	iaQuaterniond quat;
	quat.setEuler(0.1, 0.2, 0.3);

	EXPECT_NEAR(quat._x, 0.0342708, 0.000001);
	EXPECT_NEAR(quat._y, 0.106021, 0.000001);
	EXPECT_NEAR(quat._z, 0.143572, 0.000001);
	EXPECT_NEAR(quat._w, 0.983347, 0.000001);
}

TEST(QuaternionTests, GetEuler)
{
	iaQuaterniond quat;
	quat.setEuler(0.1, 0.2, 0.3);

	float64 x, y, z;
	quat.getEuler(x, y, z);

	EXPECT_NEAR(x, 0.1, 0.00000001);
	EXPECT_NEAR(y, 0.2, 0.00000001);
	EXPECT_NEAR(z, 0.3, 0.00000001);
}