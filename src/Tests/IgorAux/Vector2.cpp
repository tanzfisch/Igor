#include <gtest/gtest.h>
#include <iaVector2.h>
using namespace IgorAux;

TEST(Vector2Tests, Initial)
{
	iaVector2d vec;

	EXPECT_EQ(vec._x, 0);
	EXPECT_EQ(vec._y, 0);
}

TEST(Vector2Tests, Convert)
{
	iaVector2d vec(1,2);

	iaVector2f vec2 = vec.convert<float32>();

	EXPECT_EQ(vec2._x, 1.0f);
	EXPECT_EQ(vec2._y, 2.0f);
}

TEST(Vector2Tests,Vector2dParamCtor)
{
	iaVector2d vec(1.0, 2.0);

	EXPECT_TRUE(vec._x == 1.0 &&
		vec._y == 2.0);
}

TEST(Vector2Tests,Vector2dLenghtZero)
{
	iaVector2d vec;
	float64 lenght = vec.length();

	EXPECT_EQ(lenght, 0.0);
}

TEST(Vector2Tests,Vector2dLenght)
{
	iaVector2d vec(3.0, 4.0);
	float64 lenght = vec.length();

	EXPECT_EQ(lenght, 5.0);
}

TEST(Vector2Tests,Vector2dLenght2)
{
	iaVector2d vec(3.0, 4.0);
	float64 lenght = vec.length2();

	EXPECT_EQ(lenght, 25.0);
}

TEST(Vector2Tests,Vector2dDistance)
{
	iaVector2d vec1(4.0, 5.0);
	iaVector2d vec2(1.0, 1.0);
	float64 distance = vec1.distance(vec2);

	EXPECT_EQ(distance, 5.0);
}

TEST(Vector2Tests,Vector2dDistance2)
{
	iaVector2d vec1(4.0, 5.0);
	iaVector2d vec2(1.0, 1.0);
	float64 distance = vec1.distance2(vec2);

	EXPECT_EQ(distance, 25.0);
}

TEST(Vector2Tests,Vector2dAngleX)
{
	iaVector2d vec(1.0, 0.0);
	float64 angle = vec.angle();

	EXPECT_EQ(angle, static_cast<float64>(M_PI) * 0.5);
}

TEST(Vector2Tests,Vector2dAngle)
{
	iaVector2d vec1(1.0, 0.0);
	iaVector2d vec2(0.0, 1.0);
	float64 angle = vec1.angle(vec2);

	EXPECT_EQ(angle, static_cast<float64>(M_PI) * 0.5);
}

TEST(Vector2Tests,Vector2dSet)
{
	iaVector2d vec;
	vec.set(1, 2);

	EXPECT_TRUE(vec._x == 1.0 &&
		vec._y == 2.0);
}

TEST(Vector2Tests,Vector2dGetData)
{
	float64* data = 0;
	iaVector2d vec(2, 3);
	data = vec.getData();

	EXPECT_TRUE(data[0] == 2.0 &&
		data[1] == 3.0);
}

TEST(Vector2Tests,Vector2dRotateXY)
{
	iaVector2d vec(1.0, 0.0);
	vec.rotateXY(0.5 * static_cast<float64>(M_PI));

	EXPECT_TRUE(fabs(vec._x) < 0.0000000001 &&
		fabs(vec._y) > 0.9999999999);
}

TEST(Vector2Tests,Vector2dNormalize)
{
	iaVector2d vec(10.2, 0.0);
	vec.normalize();

	EXPECT_TRUE(fabs(vec._x) > 0.9999999999 &&
		fabs(vec._x) <= 1.0 &&
		vec._y == 0);
}
