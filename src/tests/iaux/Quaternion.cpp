#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/math/iaQuaternion.h>
using namespace iaux;

IAUX_TEST(QuaternionTests, Initial)
{
	iaQuaterniond quat;

	IAUX_EXPECT_EQUAL(quat._x, 0);
	IAUX_EXPECT_EQUAL(quat._y, 0);
	IAUX_EXPECT_EQUAL(quat._z, 0);
	IAUX_EXPECT_EQUAL(quat._w, 1);
}

IAUX_TEST(QuaternionTests, Initial2)
{
	iaQuaterniond quat(0, 0, 0, 1);

	IAUX_EXPECT_EQUAL(quat._x, 0);
	IAUX_EXPECT_EQUAL(quat._y, 0);
	IAUX_EXPECT_EQUAL(quat._z, 0);
	IAUX_EXPECT_EQUAL(quat._w, 1);
}

IAUX_TEST(QuaternionTests, InitialEuler)
{
	iaQuaterniond quat(0.2, 0.3, -1.5);

	iaVector3d vec;
	quat.getEuler(vec);

	IAUX_EXPECT_NEAR(vec._x, 0.2, 0.00000001);
	IAUX_EXPECT_NEAR(vec._y, 0.3, 0.00000001);
	IAUX_EXPECT_NEAR(vec._z, -1.5, 0.00000001);
}

IAUX_TEST(QuaternionTests, InitialEuler2)
{
	iaVector3d vec(0.2, 0.3, -1.5);

	iaQuaterniond quat(vec);
	quat.getEuler(vec);

	IAUX_EXPECT_NEAR(vec._x, 0.2, 0.00000001);
	IAUX_EXPECT_NEAR(vec._y, 0.3, 0.00000001);
	IAUX_EXPECT_NEAR(vec._z, -1.5, 0.00000001);
}

IAUX_TEST(QuaternionTests, InitialByAxisAngle1)
{
	iaQuaterniond quat(iaVector3d(1.0, 0.0, 0.0), 0.0);

	IAUX_EXPECT_EQUAL(quat._x, 0);
	IAUX_EXPECT_EQUAL(quat._y, 0);
	IAUX_EXPECT_EQUAL(quat._z, 0);
	IAUX_EXPECT_EQUAL(quat._w, 1);
}

IAUX_TEST(QuaternionTests, InitialByAxisAngle2)
{
	iaQuaterniond quat(iaVector3d(1.0, 0.0, 0.0), 0.5 * M_PI);

	IAUX_EXPECT_NEAR(quat._x, 0.707107, 0.000001);
	IAUX_EXPECT_NEAR(quat._y, 0, 0.000001);
	IAUX_EXPECT_NEAR(quat._z, 0, 0.000001);
	IAUX_EXPECT_NEAR(quat._w, 0.707107, 0.000001);
}

IAUX_TEST(QuaternionTests, InitialByAxisAngle3)
{
	iaQuaterniond quat(iaVector3d(1.0, 0.0, 0.0), M_PI);

	IAUX_EXPECT_NEAR(quat._x, 1, 0.000001);
	IAUX_EXPECT_NEAR(quat._y, 0, 0.000001);
	IAUX_EXPECT_NEAR(quat._z, 0, 0.000001);
	IAUX_EXPECT_NEAR(quat._w, 0, 0.000001);
}

IAUX_TEST(QuaternionTests, InitialByAxisAngle4)
{
	iaQuaterniond quat(iaVector3d(0.0, 1.0, 0.0), 0.5 * M_PI);

	IAUX_EXPECT_NEAR(quat._x, 0, 0.000001);
	IAUX_EXPECT_NEAR(quat._y, 0.707107, 0.000001);
	IAUX_EXPECT_NEAR(quat._z, 0, 0.000001);
	IAUX_EXPECT_NEAR(quat._w, 0.707107, 0.000001);
}

IAUX_TEST(QuaternionTests, SetAxisAngle)
{
	iaQuaterniond quat;
	quat.setAxisAngle(iaVector3d(1.0, 0.0, 0.0), 0.0);

	IAUX_EXPECT_EQUAL(quat._x, 0);
	IAUX_EXPECT_EQUAL(quat._y, 0);
	IAUX_EXPECT_EQUAL(quat._z, 0);
	IAUX_EXPECT_EQUAL(quat._w, 1);
}

IAUX_TEST(QuaternionTests, SetEuler)
{
	iaQuaterniond quat;
	quat.setEuler(0.1, 0.2, 0.3);

	IAUX_EXPECT_NEAR(quat._x, 0.0342708, 0.000001);
	IAUX_EXPECT_NEAR(quat._y, 0.106021, 0.000001);
	IAUX_EXPECT_NEAR(quat._z, 0.143572, 0.000001);
	IAUX_EXPECT_NEAR(quat._w, 0.983347, 0.000001);
}

IAUX_TEST(QuaternionTests, GetEuler)
{
	iaQuaterniond quat;
	quat.setEuler(0.1, 0.2, 0.3);

	float64 x, y, z;
	quat.getEuler(x, y, z);

	IAUX_EXPECT_NEAR(x, 0.1, 0.00000001);
	IAUX_EXPECT_NEAR(y, 0.2, 0.00000001);
	IAUX_EXPECT_NEAR(z, 0.3, 0.00000001);
}

IAUX_TEST(QuaternionTests, SetEulerVec)
{
	iaVector3d rotation(0.1, 0.2, 0.3);

	iaQuaterniond quat;
	quat.setEuler(rotation);

	IAUX_EXPECT_NEAR(quat._x, 0.0342708, 0.000001);
	IAUX_EXPECT_NEAR(quat._y, 0.106021, 0.000001);
	IAUX_EXPECT_NEAR(quat._z, 0.143572, 0.000001);
	IAUX_EXPECT_NEAR(quat._w, 0.983347, 0.000001);
}

IAUX_TEST(QuaternionTests, GetEulerVec)
{
	iaVector3d rotation(0.1, 0.2, 0.3);

	iaQuaterniond quat;
	quat.setEuler(rotation);
	quat.getEuler(rotation);

	IAUX_EXPECT_NEAR(rotation._x, 0.1, 0.00000001);
	IAUX_EXPECT_NEAR(rotation._y, 0.2, 0.00000001);
	IAUX_EXPECT_NEAR(rotation._z, 0.3, 0.00000001);
}

IAUX_TEST(QuaternionTests, SetDirect)
{
	iaQuaterniond quat;
	quat.set(0,0,0,1);

	IAUX_EXPECT_EQUAL(quat._x, 0);
	IAUX_EXPECT_EQUAL(quat._y, 0);
	IAUX_EXPECT_EQUAL(quat._z, 0);
	IAUX_EXPECT_EQUAL(quat._w, 1);
}

IAUX_TEST(QuaternionTests, Negate)
{
	iaQuaterniond quat(iaVector3d(0.0, 1.0, 0.0), 0.5 * M_PI);
	quat.negate();

	IAUX_EXPECT_NEAR(quat._x, 0, 0.000001);
	IAUX_EXPECT_NEAR(quat._y, -0.707107, 0.000001);
	IAUX_EXPECT_NEAR(quat._z, 0, 0.000001);
	IAUX_EXPECT_NEAR(quat._w, 0.707107, 0.000001);
}

IAUX_TEST(QuaternionTests, Identity)
{
	iaQuaterniond quat;
	quat.identity();

	IAUX_EXPECT_EQUAL(quat._x, 0);
	IAUX_EXPECT_EQUAL(quat._y, 0);
	IAUX_EXPECT_EQUAL(quat._z, 0);
	IAUX_EXPECT_EQUAL(quat._w, 1);
}

// TODO #234
/*IAUX_TEST(QuaternionTests, GetAxisAngle)
{
	iaQuaterniond quat(iaVector3d(0.0, 1.0, 0.0), 0.5 * M_PI);

	iaVector3d axis;
	float64 angle;
	quat.getAxisAngle(axis, angle);

	IAUX_EXPECT_NEAR(axis._x, 0.0, 0.000001);
	IAUX_EXPECT_NEAR(axis._y, 1.0, 0.000001);
	IAUX_EXPECT_NEAR(axis._z, 0.0, 0.000001);
	IAUX_EXPECT_NEAR(angle, 0.5 * M_PI, 0.000001);
}*/

IAUX_TEST(QuaternionTests, Euqal)
{
	iaQuaterniond a(iaVector3d(0.0, 1.0, 0.0), 0.5 * M_PI);
	iaQuaterniond b(iaVector3d(0.0, 1.0, 0.0), 0.5 * M_PI);

	IAUX_EXPECT_EQUAL(a, b);
}

IAUX_TEST(QuaternionTests, NotEuqal)
{
	iaQuaterniond a(iaVector3d(0.0, 1.0, 0.0), 0.5 * M_PI);
	iaQuaterniond b(iaVector3d(1.0, 0.0, 0.0), 0.5 * M_PI);

	IAUX_EXPECT_NOT_EQUAL(a, b);
}