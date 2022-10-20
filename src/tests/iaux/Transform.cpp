#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/math/iaTransform.h>
using namespace iaux;

IAUX_TEST(TransformTests, Initial)
{
	iaTransformd trans;

	IAUX_EXPECT_EQUAL(trans._translate, iaVector3d());
	IAUX_EXPECT_EQUAL(trans._scale, iaVector3d(1,1,1));
	IAUX_EXPECT_EQUAL(trans._orientation, iaQuaterniond(0,0,0,1));
}
