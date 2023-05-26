#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/math/iaVector2.h>
#include <iaux/math/iaVector3.h>
#include <iaux/math/iaVector4.h>
#include <iaux/data/iaColor4.h>
#include <iaux/data/iaColor3.h>

#include <iaux/math/iaMath.h>
using namespace iaux;

IAUX_TEST(MathTests, LerpFloat32)
{
	const float32 val1 = -2.0;
	const float32 val2 = 5.0;
	const float32 result = iaMath::lerp(val1, val2, 0.5);

	IAUX_EXPECT_NEAR(result, 1.5, 0.000001);
}

IAUX_TEST(MathTests, LerpFloat64)
{
	const float64 val1 = -2.0;
	const float64 val2 = 5.0;
	const float64 result = iaMath::lerp(val1, val2, 0.5);

	IAUX_EXPECT_NEAR(result, 1.5, 0.000001);
}

IAUX_TEST(MathTests, LerpUInt32)
{
	const uint32 val1 = 1;
	const uint32 val2 = 5;
	const uint32 result = iaMath::lerp(val1, val2, 0.5);

	IAUX_EXPECT_EQUAL(result, 3);
}

IAUX_TEST(MathTests, LerpInt32)
{
	const int32 val1 = -2;
	const int32 val2 = 5;
	const int32 result = iaMath::lerp(val1, val2, 0.5);

	IAUX_EXPECT_EQUAL(result, 2);
}

IAUX_TEST(MathTests, LerpUInt64)
{
	const uint64 val1 = 1;
	const uint64 val2 = 5;
	const uint64 result = iaMath::lerp(val1, val2, 0.5);

	IAUX_EXPECT_EQUAL(result, 3);
}

IAUX_TEST(MathTests, LerpInt64)
{
	const int64 val1 = -2;
	const int64 val2 = 5;
	const int64 result = iaMath::lerp(val1, val2, 0.5);

	IAUX_EXPECT_EQUAL(result, 2);
}

IAUX_TEST(MathTests, LerpColor3f)
{
	const iaColor3f color1(0.1, 0.12, 1.0);
	const iaColor3f color2(0.8, 0.7, 1.0);
	const iaColor3f color = iaMath::lerp(color1, color2, 0.5);

	IAUX_EXPECT_NEAR(color._r, 0.45, 0.000001);
	IAUX_EXPECT_NEAR(color._g, 0.41, 0.000001);
	IAUX_EXPECT_NEAR(color._b, 1.0, 0.000001);
}

IAUX_TEST(MathTests, LerpColor3c)
{
	const iaColor3c color1(0, 128, 200);
	const iaColor3c color2(84, 128, 180);
	const iaColor3c color = iaMath::lerp(color1, color2, 0.5);

	IAUX_EXPECT_EQUAL(color._r, 42);
	IAUX_EXPECT_EQUAL(color._g, 128);
	IAUX_EXPECT_EQUAL(color._b, 190);
}

IAUX_TEST(MathTests, LerpColor4f)
{
	const iaColor4f color1(0.1, 0.12, 1.0, 0.2);
	const iaColor4f color2(0.8, 0.7, 1.0, 0.6);
	const iaColor4f color = iaMath::lerp(color1, color2, 0.5);

	IAUX_EXPECT_NEAR(color._r, 0.45, 0.0001);
	IAUX_EXPECT_NEAR(color._g, 0.41, 0.0001);
	IAUX_EXPECT_NEAR(color._b, 1.0, 0.0001);
	IAUX_EXPECT_NEAR(color._a, 0.4, 0.0001);
}

IAUX_TEST(MathTests, LerpColor4c)
{
	const iaColor4c color1(0, 128, 200, 50);
	const iaColor4c color2(84, 128, 180, 70);

	const iaColor4c color = iaMath::lerp(color1, color2, 0.5);

	IAUX_EXPECT_EQUAL(color._r, 42);
	IAUX_EXPECT_EQUAL(color._g, 128);
	IAUX_EXPECT_EQUAL(color._b, 190);
	IAUX_EXPECT_EQUAL(color._a, 60);
}

IAUX_TEST(MathTests, LerpVector2f)
{
	const iaVector2f vec1(0, 5);
	const iaVector2f vec2(-13, 12);

	const iaVector2f result = iaMath::lerp(vec1, vec2, 0.5);

	IAUX_EXPECT_NEAR(result._x, -6.5, 0.0001);
	IAUX_EXPECT_NEAR(result._y, 8.5, 0.0001);
}

IAUX_TEST(MathTests, LerpVector2d)
{
	const iaVector2d vec1(0, 5);
	const iaVector2d vec2(-13, 12);

	const iaVector2d result = iaMath::lerp(vec1, vec2, 0.5);

	IAUX_EXPECT_NEAR(result._x, -6.5, 0.0001);
	IAUX_EXPECT_NEAR(result._y, 8.5, 0.0001);
}

IAUX_TEST(MathTests, LerpVector3f)
{
	const iaVector3f vec1(0, 5, 42);
	const iaVector3f vec2(-13, 12, -45);

	const iaVector3f result = iaMath::lerp(vec1, vec2, 0.5);

	IAUX_EXPECT_NEAR(result._x, -6.5, 0.0001);
	IAUX_EXPECT_NEAR(result._y, 8.5, 0.0001);
	IAUX_EXPECT_NEAR(result._z, -1.5, 0.0001);
}

IAUX_TEST(MathTests, LerpVector3d)
{
	const iaVector3d vec1(0, 5, -7);
	const iaVector3d vec2(-13, 12, 8);

	const iaVector3d result = iaMath::lerp(vec1, vec2, 0.5);

	IAUX_EXPECT_NEAR(result._x, -6.5, 0.0001);
	IAUX_EXPECT_NEAR(result._y, 8.5, 0.0001);
	IAUX_EXPECT_NEAR(result._z, 0.5, 0.0001);
}

IAUX_TEST(MathTests, LerpVector4f)
{
	const iaVector4f vec1(0, 5, 42, 0);
	const iaVector4f vec2(-13, 12, -45, 0);

	const iaVector4f result = iaMath::lerp(vec1, vec2, 0.5);

	IAUX_EXPECT_NEAR(result._x, -6.5, 0.0001);
	IAUX_EXPECT_NEAR(result._y, 8.5, 0.0001);
	IAUX_EXPECT_NEAR(result._z, -1.5, 0.0001);
	IAUX_EXPECT_EQUAL(result._w, 0.0);
}

IAUX_TEST(MathTests, LerpVector4d)
{
	const iaVector4d vec1(0, 5, -7, 1);
	const iaVector4d vec2(-13, 12, 8, 1);

	const iaVector4d result = iaMath::lerp(vec1, vec2, 0.5);

	IAUX_EXPECT_NEAR(result._x, -6.5, 0.0001);
	IAUX_EXPECT_NEAR(result._y, 8.5, 0.0001);
	IAUX_EXPECT_NEAR(result._z, 0.5, 0.0001);
	IAUX_EXPECT_EQUAL(result._w, 1.0);
}