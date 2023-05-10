#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/math/iaVector2.h>
using namespace iaux;

IAUX_TEST(Vector2Tests, Initial)
{
	iaVector2d vec;

	IAUX_EXPECT_EQUAL(vec._x, 0);
	IAUX_EXPECT_EQUAL(vec._y, 0);
}

IAUX_TEST(Vector2Tests, Convert)
{
	iaVector2d vec(1, 2);

	iaVector2f vec2 = vec.convert<float32>();

	IAUX_EXPECT_EQUAL(vec2._x, 1.0f);
	IAUX_EXPECT_EQUAL(vec2._y, 2.0f);
}

IAUX_TEST(Vector2Tests, Vector2dParamCtor)
{
	iaVector2d vec(1.0, 2.0);

	IAUX_EXPECT_TRUE(vec._x == 1.0 &&
				vec._y == 2.0);
}

IAUX_TEST(Vector2Tests, Vector2dLenghtZero)
{
	iaVector2d vec;
	float64 lenght = vec.length();

	IAUX_EXPECT_EQUAL(lenght, 0.0);
}

IAUX_TEST(Vector2Tests, Vector2dLenght)
{
	iaVector2d vec(3.0, 4.0);
	float64 lenght = vec.length();

	IAUX_EXPECT_EQUAL(lenght, 5.0);
}

IAUX_TEST(Vector2Tests, Vector2dLenght2)
{
	iaVector2d vec(3.0, 4.0);
	float64 lenght = vec.length2();

	IAUX_EXPECT_EQUAL(lenght, 25.0);
}

IAUX_TEST(Vector2Tests, Vector2dDistance)
{
	iaVector2d vec1(4.0, 5.0);
	iaVector2d vec2(1.0, 1.0);
	float64 distance = vec1.distance(vec2);

	IAUX_EXPECT_EQUAL(distance, 5.0);
}

IAUX_TEST(Vector2Tests, Vector2dDistance2)
{
	iaVector2d vec1(4.0, 5.0);
	iaVector2d vec2(1.0, 1.0);
	float64 distance = vec1.distance2(vec2);

	IAUX_EXPECT_EQUAL(distance, 25.0);
}

IAUX_TEST(Vector2Tests, Vector2dAngleX)
{
	iaVector2d vec1(0.0, 1.0);
	float64 angle1 = vec1.angle();

	IAUX_EXPECT_EQUAL(angle1, 90.0 * IGOR_GRAD2RAD);

	iaVector2d vec2(1.0, 0.0);
	float64 angle2 = vec2.angle();

	IAUX_EXPECT_EQUAL(angle2, 0.0 * IGOR_GRAD2RAD);	

	iaVector2d vec3(0.0, -1.0);
	float64 angle3 = vec3.angle();

	IAUX_EXPECT_EQUAL(angle3, -90.0 * IGOR_GRAD2RAD);

	iaVector2d vec4(-1.0, 0.0);
	float64 angle4 = vec4.angle();

	IAUX_EXPECT_EQUAL(angle4, 180.0 * IGOR_GRAD2RAD);	

}

IAUX_TEST(Vector2Tests, Vector2dAngle)
{
	iaVector2d vec1(1.0, 0.0);
	iaVector2d vec2(0.0, 1.0);
	float64 angle = vec1.angle(vec2);

	IAUX_EXPECT_EQUAL(angle, static_cast<float64>(M_PI) * 0.5);
}

IAUX_TEST(Vector2Tests, Vector2dSet)
{
	iaVector2d vec;
	vec.set(1, 2);

	IAUX_EXPECT_TRUE(vec._x == 1.0 &&
				vec._y == 2.0);
}

IAUX_TEST(Vector2Tests, Vector2dGetData)
{
	float64 *data = 0;
	iaVector2d vec(2, 3);
	data = vec.getData();

	IAUX_EXPECT_TRUE(data[0] == 2.0 &&
				data[1] == 3.0);
}

IAUX_TEST(Vector2Tests, Vector2dRotateXY)
{
	iaVector2d vec(1.0, 0.0);
	vec.rotateXY(0.5 * static_cast<float64>(M_PI));

	IAUX_EXPECT_TRUE(fabs(vec._x) < 0.0000000001 &&
				fabs(vec._y) > 0.9999999999);
}

IAUX_TEST(Vector2Tests, Vector2dNormalize)
{
	iaVector2d vec(10.2, 0.0);
	vec.normalize();

	IAUX_EXPECT_TRUE(fabs(vec._x) > 0.9999999999 &&
				fabs(vec._x) <= 1.0 &&
				vec._y == 0);
}
