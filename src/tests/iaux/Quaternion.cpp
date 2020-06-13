#include <gtest/gtest.h>

#include <iaux/math/iaQuaternion.h>
using namespace iaux;

TEST(QuaternionTests, Initial)
{
	iaQuaterniond quat;

	EXPECT_EQ(quat._x, 0);
	EXPECT_EQ(quat._y, 0);
	EXPECT_EQ(quat._z, 0);
	EXPECT_EQ(quat._w, 1);
}

TEST(QuaternionTests, Initial2)
{
	iaQuaterniond quat(0, 0, 0, 1);

	EXPECT_EQ(quat._x, 0);
	EXPECT_EQ(quat._y, 0);
	EXPECT_EQ(quat._z, 0);
	EXPECT_EQ(quat._w, 1);
}

TEST(QuaternionTests, InitialEuler)
{
	iaQuaterniond quat(0.2, 0.3, -1.5);

	iaVector3d vec;
	quat.getEuler(vec);

	EXPECT_NEAR(vec._x, 0.2, 0.00000001);
	EXPECT_NEAR(vec._y, 0.3, 0.00000001);
	EXPECT_NEAR(vec._z, -1.5, 0.00000001);
}

TEST(QuaternionTests, InitialEuler2)
{
	iaVector3d vec(0.2, 0.3, -1.5);

	iaQuaterniond quat(vec);
	quat.getEuler(vec);

	EXPECT_NEAR(vec._x, 0.2, 0.00000001);
	EXPECT_NEAR(vec._y, 0.3, 0.00000001);
	EXPECT_NEAR(vec._z, -1.5, 0.00000001);
}

TEST(QuaternionTests, InitialByAxisAngle1)
{
	iaQuaterniond quat(iaVector3d(1.0, 0.0, 0.0), 0.0);

	EXPECT_EQ(quat._x, 0);
	EXPECT_EQ(quat._y, 0);
	EXPECT_EQ(quat._z, 0);
	EXPECT_EQ(quat._w, 1);
}

TEST(QuaternionTests, InitialByAxisAngle2)
{
	iaQuaterniond quat(iaVector3d(1.0, 0.0, 0.0), 0.5 * M_PI);

	EXPECT_NEAR(quat._x, 0.707107, 0.000001);
	EXPECT_NEAR(quat._y, 0, 0.000001);
	EXPECT_NEAR(quat._z, 0, 0.000001);
	EXPECT_NEAR(quat._w, 0.707107, 0.000001);
}

TEST(QuaternionTests, InitialByAxisAngle3)
{
	iaQuaterniond quat(iaVector3d(1.0, 0.0, 0.0), M_PI);

	EXPECT_NEAR(quat._x, 1, 0.000001);
	EXPECT_NEAR(quat._y, 0, 0.000001);
	EXPECT_NEAR(quat._z, 0, 0.000001);
	EXPECT_NEAR(quat._w, 0, 0.000001);
}

TEST(QuaternionTests, InitialByAxisAngle4)
{
	iaQuaterniond quat(iaVector3d(0.0, 1.0, 0.0), 0.5 * M_PI);

	EXPECT_NEAR(quat._x, 0, 0.000001);
	EXPECT_NEAR(quat._y, 0.707107, 0.000001);
	EXPECT_NEAR(quat._z, 0, 0.000001);
	EXPECT_NEAR(quat._w, 0.707107, 0.000001);
}

TEST(QuaternionTests, SetAxisAngle)
{
	iaQuaterniond quat;
	quat.setAxisAngle(iaVector3d(1.0, 0.0, 0.0), 0.0);

	EXPECT_EQ(quat._x, 0);
	EXPECT_EQ(quat._y, 0);
	EXPECT_EQ(quat._z, 0);
	EXPECT_EQ(quat._w, 1);
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

TEST(QuaternionTests, SetEulerVec)
{
	iaVector3d rotation(0.1, 0.2, 0.3);

	iaQuaterniond quat;
	quat.setEuler(rotation);

	EXPECT_NEAR(quat._x, 0.0342708, 0.000001);
	EXPECT_NEAR(quat._y, 0.106021, 0.000001);
	EXPECT_NEAR(quat._z, 0.143572, 0.000001);
	EXPECT_NEAR(quat._w, 0.983347, 0.000001);
}

TEST(QuaternionTests, GetEulerVec)
{
	iaVector3d rotation(0.1, 0.2, 0.3);

	iaQuaterniond quat;
	quat.setEuler(rotation);
	quat.getEuler(rotation);

	EXPECT_NEAR(rotation._x, 0.1, 0.00000001);
	EXPECT_NEAR(rotation._y, 0.2, 0.00000001);
	EXPECT_NEAR(rotation._z, 0.3, 0.00000001);
}

TEST(QuaternionTests, SetDirect)
{
	iaQuaterniond quat;
	quat.set(0,0,0,1);

	EXPECT_EQ(quat._x, 0);
	EXPECT_EQ(quat._y, 0);
	EXPECT_EQ(quat._z, 0);
	EXPECT_EQ(quat._w, 1);
}

TEST(QuaternionTests, Negate)
{
	iaQuaterniond quat(iaVector3d(0.0, 1.0, 0.0), 0.5 * M_PI);
	quat.negate();

	EXPECT_NEAR(quat._x, 0, 0.000001);
	EXPECT_NEAR(quat._y, -0.707107, 0.000001);
	EXPECT_NEAR(quat._z, 0, 0.000001);
	EXPECT_NEAR(quat._w, 0.707107, 0.000001);
}

TEST(QuaternionTests, Identity)
{
	iaQuaterniond quat;
	quat.identity();

	EXPECT_EQ(quat._x, 0);
	EXPECT_EQ(quat._y, 0);
	EXPECT_EQ(quat._z, 0);
	EXPECT_EQ(quat._w, 1);
}

TEST(QuaternionTests, GetAxisAngle)
{
	iaQuaterniond quat(iaVector3d(0.0, 1.0, 0.0), 0.5 * M_PI);

	iaVector3d axis;
	float64 angle;
	quat.getAxisAngle(axis, angle);

	EXPECT_NEAR(axis._x, 0.0, 0.000001);
	EXPECT_NEAR(axis._y, 1.0, 0.000001);
	EXPECT_NEAR(axis._z, 0.0, 0.000001);
	EXPECT_NEAR(angle, 0.5 * M_PI, 0.000001);
}

TEST(QuaternionTests, Euqal)
{
	iaQuaterniond a(iaVector3d(0.0, 1.0, 0.0), 0.5 * M_PI);
	iaQuaterniond b(iaVector3d(0.0, 1.0, 0.0), 0.5 * M_PI);

	EXPECT_EQ(a, b);
}

TEST(QuaternionTests, NotEuqal)
{
	iaQuaterniond a(iaVector3d(0.0, 1.0, 0.0), 0.5 * M_PI);
	iaQuaterniond b(iaVector3d(1.0, 0.0, 0.0), 0.5 * M_PI);

	EXPECT_NE(a, b);
}