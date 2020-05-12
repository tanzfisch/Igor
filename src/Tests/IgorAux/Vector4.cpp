#include <gtest/gtest.h>
#include <iaVector4.h>
using namespace IgorAux;

TEST(Vector4Tests, Initial)
{
	iaVector4d vec;

	EXPECT_EQ(vec._x, 0);
	EXPECT_EQ(vec._y, 0);
	EXPECT_EQ(vec._z, 0);
	EXPECT_EQ(vec._w, 1);
}

TEST(Vector4Tests, Convert)
{
	iaVector4d vec(1, 2, 3, 4);
	iaVector4f vec2 = vec.convert<float32>();

	EXPECT_EQ(vec2._x, 1.0f);
	EXPECT_EQ(vec2._y, 2.0f);
	EXPECT_EQ(vec2._z, 3.0f);
	EXPECT_EQ(vec2._w, 4.0f);
}