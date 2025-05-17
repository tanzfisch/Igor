#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/math/iaQuaternion.h>
using namespace iaux;

IAUX_TEST(QuaternionTests, Initial)
{
	iaQuaterniond quat;

	IAUX_EXPECT_EQUAL(quat._w, 1);
	IAUX_EXPECT_EQUAL(quat._x, 0);
	IAUX_EXPECT_EQUAL(quat._y, 0);
	IAUX_EXPECT_EQUAL(quat._z, 0);
}

IAUX_TEST(QuaternionTests, Initial2)
{
	iaQuaterniond quat(1, 0, 0, 0);

	IAUX_EXPECT_EQUAL(quat._w, 1);
	IAUX_EXPECT_EQUAL(quat._x, 0);
	IAUX_EXPECT_EQUAL(quat._y, 0);
	IAUX_EXPECT_EQUAL(quat._z, 0);
}

IAUX_TEST(QuaternionTests, InitialEuler)
{
	iaQuaterniond quat = iaQuaterniond::fromEuler(0.2, 0.3, -1.5);

	iaVector3d vec = quat.toEuler();

	IAUX_EXPECT_NEAR(vec._x, 0.2, 0.01);
	IAUX_EXPECT_NEAR(vec._y, 0.3, 0.01);
	IAUX_EXPECT_NEAR(vec._z, -1.5, 0.01);
}

IAUX_TEST(QuaternionTests, InitialEuler2)
{
	iaQuaterniond quat(iaQuaterniond::fromEuler(0.2, -0.9, -1.5));
	iaVector3d vec = quat.toEuler();

	IAUX_EXPECT_NEAR(vec._x, 0.2, 0.00000001);
	IAUX_EXPECT_NEAR(vec._y, -0.9, 0.00000001);
	IAUX_EXPECT_NEAR(vec._z, -1.5, 0.00000001);
}

IAUX_TEST(QuaternionTests, InitialByAxisAngle2)
{
	iaQuaterniond quat = iaQuaterniond::fromAxisAngle(iaVector3d(1.0, 0.0, 0.0), 0.5 * M_PI);

	IAUX_EXPECT_NEAR(quat._x, 0.707107, 0.000001);
	IAUX_EXPECT_NEAR(quat._y, 0, 0.000001);
	IAUX_EXPECT_NEAR(quat._z, 0, 0.000001);
	IAUX_EXPECT_NEAR(quat._w, 0.707107, 0.000001);
}

IAUX_TEST(QuaternionTests, InitialByAxisAngle3)
{
	iaQuaterniond quat = iaQuaterniond::fromAxisAngle(iaVector3d(1.0, 0.0, 0.0), M_PI);

	IAUX_EXPECT_NEAR(quat._x, 1, 0.000001);
	IAUX_EXPECT_NEAR(quat._y, 0, 0.000001);
	IAUX_EXPECT_NEAR(quat._z, 0, 0.000001);
	IAUX_EXPECT_NEAR(quat._w, 0, 0.000001);
}

IAUX_TEST(QuaternionTests, InitialByAxisAngle4)
{
	iaQuaterniond quat = iaQuaterniond::fromAxisAngle(iaVector3d(0.0, 1.0, 0.0), 0.5 * M_PI);

	IAUX_EXPECT_NEAR(quat._x, 0, 0.000001);
	IAUX_EXPECT_NEAR(quat._y, 0.707107, 0.000001);
	IAUX_EXPECT_NEAR(quat._z, 0, 0.000001);
	IAUX_EXPECT_NEAR(quat._w, 0.707107, 0.000001);
}

IAUX_TEST(QuaternionTests, SetAxisAngle)
{
	iaQuaterniond quat = iaQuaterniond::fromAxisAngle(iaVector3d(1.0, 0.0, 0.0), 0.0);

	IAUX_EXPECT_EQUAL(quat._x, 0);
	IAUX_EXPECT_EQUAL(quat._y, 0);
	IAUX_EXPECT_EQUAL(quat._z, 0);
	IAUX_EXPECT_EQUAL(quat._w, 1);
}

IAUX_TEST(QuaternionTests, SetEuler)
{
	iaQuaterniond quat = iaQuaterniond::fromEuler(0.1, 0.2, 0.3);

	auto euler = quat.toEuler();
	IAUX_EXPECT_NEAR(euler._x, 0.1, 0.000001);
	IAUX_EXPECT_NEAR(euler._y, 0.2, 0.000001);
	IAUX_EXPECT_NEAR(euler._z, 0.3, 0.000001);
}

IAUX_TEST(QuaternionTests, SetEulerVec)
{
	iaVector3d rotation(0.1, -0.2, 0.3);

	iaQuaterniond quat = iaQuaterniond::fromEuler(rotation);

	auto euler = quat.toEuler();
	IAUX_EXPECT_NEAR(euler._x, 0.1, 0.000001);
	IAUX_EXPECT_NEAR(euler._y, -0.2, 0.000001);
	IAUX_EXPECT_NEAR(euler._z, 0.3, 0.000001);
}

IAUX_TEST(QuaternionTests, GetEulerVec)
{
	iaVector3d rotation(0.1, -0.2, 0.3);

	iaQuaterniond quat = iaQuaterniond::fromEuler(rotation);
	rotation = quat.toEuler();

	IAUX_EXPECT_NEAR(rotation._x, 0.1, 0.00000001);
	IAUX_EXPECT_NEAR(rotation._y, -0.2, 0.00000001);
	IAUX_EXPECT_NEAR(rotation._z, 0.3, 0.00000001);
}

IAUX_TEST(QuaternionTests, SetDirect)
{
	iaQuaterniond quat;
	quat.set(1, 0, 0, 0);

	IAUX_EXPECT_EQUAL(quat._w, 1);
	IAUX_EXPECT_EQUAL(quat._x, 0);
	IAUX_EXPECT_EQUAL(quat._y, 0);
	IAUX_EXPECT_EQUAL(quat._z, 0);
}

IAUX_TEST(QuaternionTests, Identity)
{
	iaQuaterniond quat;
	quat.identity();

	IAUX_EXPECT_EQUAL(quat._w, 1);
	IAUX_EXPECT_EQUAL(quat._x, 0);
	IAUX_EXPECT_EQUAL(quat._y, 0);
	IAUX_EXPECT_EQUAL(quat._z, 0);
}

IAUX_TEST(QuaternionTests, Euqal)
{
	iaQuaterniond a = iaQuaterniond::fromAxisAngle(iaVector3d(0.0, 1.0, 0.0), 0.5 * M_PI);
	iaQuaterniond b = iaQuaterniond::fromAxisAngle(iaVector3d(0.0, 1.0, 0.0), 0.5 * M_PI);

	IAUX_EXPECT_EQUAL(a, b);
}

IAUX_TEST(QuaternionTests, NotEuqal)
{
	iaQuaterniond a = iaQuaterniond::fromAxisAngle(iaVector3d(0.0, 1.0, 0.0), 0.5 * M_PI);
	iaQuaterniond b = iaQuaterniond::fromAxisAngle(iaVector3d(1.0, 0.0, 0.0), 0.5 * M_PI);

	IAUX_EXPECT_NOT_EQUAL(a, b);
}

IAUX_TEST(QuaternionTests, Multiplication)
{
	iaQuaterniond q1 = {1, 2, 3, 4};
	iaQuaterniond q2 = {5, 6, 7, 8};

	iaQuaterniond result = q1 * q2;

	// Expected result from manual multiplication
	iaQuaterniond expected = {
		-60, // w
		12,	 // x
		30,	 // y
		24	 // z
	};

	IAUX_EXPECT_EQUAL(result, expected);
}

IAUX_TEST(QuaternionTests, RotateVector90DegreesAroundY) {
    iaVector3d original = {1.0f, 0.0f, 0.0f};

    float angle = M_PI / 2.0f; // 90 degrees in radians
    iaQuaterniond q = {
        std::cos(angle / 2.0f), // w
        0.0f,                   // x
        std::sin(angle / 2.0f), // y (rotation axis)
        0.0f                    // z
    };

    iaVector3d rotated = q.rotate(original);

    iaVector3d expected = {0.0f, 0.0f, -1.0f};

    IAUX_EXPECT_NEAR(rotated._x, expected._x, 0.00001);
	IAUX_EXPECT_NEAR(rotated._y, expected._y, 0.00001);
	IAUX_EXPECT_NEAR(rotated._z, expected._z, 0.00001);
}