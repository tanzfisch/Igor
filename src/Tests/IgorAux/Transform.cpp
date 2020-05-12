#include <gtest/gtest.h>
#include <iaTransform.h>
using namespace IgorAux;

TEST(TransformTests, Initial)
{
	iaTransformd trans;

	EXPECT_EQ(trans._translate, iaVector3d());
	EXPECT_EQ(trans._scale, iaVector3d(1,1,1));
	EXPECT_EQ(trans._orientation, iaQuaterniond(0,0,0,1));
}
