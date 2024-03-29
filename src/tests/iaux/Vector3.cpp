#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/math/iaVector3.h>
using namespace iaux;

IAUX_TEST(Vector3Tests, Initial)
{
	iaVector3d vec;

	IAUX_EXPECT_EQUAL(vec._x, 0);
	IAUX_EXPECT_EQUAL(vec._y, 0);
	IAUX_EXPECT_EQUAL(vec._z, 0);
}

IAUX_TEST(Vector3Tests, Convert)
{
	iaVector3d vec(1,2,3);
	iaVector3f vec2 = vec.convert<float32>();

	IAUX_EXPECT_EQUAL(vec2._x, 1.0f);
	IAUX_EXPECT_EQUAL(vec2._y, 2.0f);
	IAUX_EXPECT_EQUAL(vec2._z, 3.0f);
}

IAUX_TEST(Vector3Tests, Vector3dParamCtor)
{
	iaVector3d vec(1.0, -2.0, 3.0);

	IAUX_EXPECT_TRUE(vec._x == 1.0 && vec._y == -2.0 && vec._z == 3.0);
}

IAUX_TEST(Vector3Tests, Vector3dLenghtZero)
{
	iaVector3d vec;
	float64 length = vec.length();

	IAUX_EXPECT_EQUAL(length, 0.0);
}

IAUX_TEST(Vector3Tests, Vector3dLenght)
{
	iaVector3d vec(3.0, 0.0, 4.0);
	float64 length = vec.length();

	IAUX_EXPECT_EQUAL(length, 5.0);
}

IAUX_TEST(Vector3Tests, Vector3dLenght2)
{
	iaVector3d vec(0.0, 3.0, 4.0);
	float64 length = vec.length2();

	IAUX_EXPECT_EQUAL(length, 25.0);
}

IAUX_TEST(Vector3Tests, Vector3dDistance)
{
	iaVector3d vec1(4.0, 5.0, 3.0);
	iaVector3d vec2(1.0, 1.0, 3.0);
	float64 distance = vec1.distance(vec2);

	IAUX_EXPECT_EQUAL(distance, 5.0);
}

IAUX_TEST(Vector3Tests, Vector3dDistance2)
{
	iaVector3d vec1(4.0, 7.0, 5.0);
	iaVector3d vec2(1.0, 7.0, 1.0);
	float64 distance = vec1.distance2(vec2);

	IAUX_EXPECT_EQUAL(distance, 25.0);
}

IAUX_TEST(Vector3Tests, Vector3dAngle)
{
	iaVector3d vec1(1.0, 0.0, 0.0);
	iaVector3d vec2(0.0, 1.0, 0.0);
	float64 angle = vec1.angle(vec2);

	IAUX_EXPECT_EQUAL(angle, static_cast<float64>(M_PI) * 0.5);
}

IAUX_TEST(Vector3Tests, Vector3dSet)
{
	iaVector3d vec;
	vec.set(1, 2, 3);

	IAUX_EXPECT_TRUE(vec._x == 1.0 && vec._y == 2.0 && vec._z == 3.0);
}

IAUX_TEST(Vector3Tests, Vector3dGetData)
{
	float64* data = 0;
	iaVector3d vec(2.0, 3.0, 17.5);
	data = vec.getData();

	IAUX_EXPECT_TRUE(data[0] == 2.0 && data[1] == 3.0 && data[2] == 17.5);
}

IAUX_TEST(Vector3Tests, Vector3dNormalize)
{
	iaVector3d vec(10.2, 0.0, 0.0);
	vec.normalize();

	IAUX_EXPECT_TRUE(fabs(vec._x) > 0.9999999999 && fabs(vec._x) <= 1.0 &&
		vec._y == 0 && vec._z == 0);
}
