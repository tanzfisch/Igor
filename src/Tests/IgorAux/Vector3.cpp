#include <gtest/gtest.h>
#include <iaVector3.h>
using namespace IgorAux;

TEST(Vector3Tests, Initial)
{
	iaVector3d vec;

	EXPECT_EQ(vec._x, 0);
	EXPECT_EQ(vec._y, 0);
	EXPECT_EQ(vec._z, 0);
}

TEST(Vector3Tests, Convert)
{
	iaVector3d vec(1,2,3);
	iaVector3f vec2 = vec.convert<float32>();

	EXPECT_EQ(vec2._x, 1.0f);
	EXPECT_EQ(vec2._y, 2.0f);
	EXPECT_EQ(vec2._z, 3.0f);
}

TEST(Vector3Tests, Vector3dParamCtor)
{
	iaVector3d vec(1.0, -2.0, 3.0);

	EXPECT_TRUE(vec._x == 1.0 && vec._y == -2.0 && vec._z == 3.0);
}

TEST(Vector3Tests, Vector3dLenghtZero)
{
	iaVector3d vec;
	float64 lenght = vec.length();

	EXPECT_EQ(lenght, 0.0);
}

TEST(Vector3Tests, Vector3dLenght)
{
	iaVector3d vec(3.0, 0.0, 4.0);
	float64 lenght = vec.length();

	EXPECT_EQ(lenght, 5.0);
}

TEST(Vector3Tests, Vector3dLenght2)
{
	iaVector3d vec(0.0, 3.0, 4.0);
	float64 lenght = vec.length2();

	EXPECT_EQ(lenght, 25.0);
}

TEST(Vector3Tests, Vector3dDistance)
{
	iaVector3d vec1(4.0, 5.0, 3.0);
	iaVector3d vec2(1.0, 1.0, 3.0);
	float64 distance = vec1.distance(vec2);

	EXPECT_EQ(distance, 5.0);
}

TEST(Vector3Tests, Vector3dDistance2)
{
	iaVector3d vec1(4.0, 7.0, 5.0);
	iaVector3d vec2(1.0, 7.0, 1.0);
	float64 distance = vec1.distance2(vec2);

	EXPECT_EQ(distance, 25.0);
}

TEST(Vector3Tests, Vector3dAngle)
{
	iaVector3d vec1(1.0, 0.0, 0.0);
	iaVector3d vec2(0.0, 1.0, 0.0);
	float64 angle = vec1.angle(vec2);

	EXPECT_EQ(angle, static_cast<float64>(M_PI) * 0.5);
}

TEST(Vector3Tests, Vector3dSet)
{
	iaVector3d vec;
	vec.set(1, 2, 3);

	EXPECT_TRUE(vec._x == 1.0 && vec._y == 2.0 && vec._z == 3.0);
}

TEST(Vector3Tests, Vector3dGetData)
{
	float64* data = 0;
	iaVector3d vec(2.0, 3.0, 17.5);
	data = vec.getData();

	EXPECT_TRUE(data[0] == 2.0 && data[1] == 3.0 && data[2] == 17.5);
}

TEST(Vector3Tests, Vector3dNormalize)
{
	iaVector3d vec(10.2, 0.0, 0.0);
	vec.normalize();

	EXPECT_TRUE(fabs(vec._x) > 0.9999999999 && fabs(vec._x) <= 1.0 &&
		vec._y == 0 && vec._z == 0);
}
