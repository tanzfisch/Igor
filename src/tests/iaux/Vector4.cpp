#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/math/iaVector4.h>
using namespace iaux;

IAUX_TEST(Vector4Tests, Initial)
{
	iaVector4d vec;

	IAUX_EXPECT_EQ(vec._x, 0);
	IAUX_EXPECT_EQ(vec._y, 0);
	IAUX_EXPECT_EQ(vec._z, 0);
	IAUX_EXPECT_EQ(vec._w, 1);
}

IAUX_TEST(Vector4Tests, Convert)
{
	iaVector4d vec(1, 2, 3, 4);
	iaVector4f vec2 = vec.convert<float32>();

	IAUX_EXPECT_EQ(vec2._x, 1.0f);
	IAUX_EXPECT_EQ(vec2._y, 2.0f);
	IAUX_EXPECT_EQ(vec2._z, 3.0f);
	IAUX_EXPECT_EQ(vec2._w, 4.0f);
}