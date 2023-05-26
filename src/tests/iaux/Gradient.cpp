#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/data/iaKeyFrameGraph.h>
using namespace iaux;

IAUX_TEST(GradientTests, Empty)
{
	iaKeyFrameGraphd gradient;

	IAUX_EXPECT_TRUE(gradient.isEmpty());
	IAUX_EXPECT_TRUE(gradient.getValues().empty());
	IAUX_EXPECT_EQUAL(gradient.getValueCount(), 0);

	gradient.setValue(100.0, 150.0);
	gradient.setValue(200.0, 250.0);
	gradient.clear();

	IAUX_EXPECT_TRUE(gradient.isEmpty());
	IAUX_EXPECT_TRUE(gradient.getValues().empty());
	IAUX_EXPECT_EQUAL(gradient.getValueCount(), 0);	
}

IAUX_TEST(GradientTests, GetValue)
{
	iaKeyFrameGraphd gradient;

	gradient.setValue(-13.0, 100.0);
	gradient.setValue(13.0, 200.0);

	float64 value1 = gradient.getValue(0.0);
	float64 value2;
	value2 = gradient.getValue(6.5);

	IAUX_EXPECT_EQUAL(value1, 150.0);
	IAUX_EXPECT_EQUAL(value2, 175.0);
}

IAUX_TEST(GradientTests, WorkWithIndex)
{
	iaKeyFrameGraphd gradient;

	gradient.setValue(100.0, 150.0);
	gradient.setValue(200.0, 250.0);
	gradient.setValueAtIndex(0, 300.0, 350.0);

	float64 at;
	float64 value;
	gradient.getValueAtIndex(1, at, value);

	IAUX_EXPECT_EQUAL(at, 300.0);
	IAUX_EXPECT_EQUAL(value, 350.0);
	IAUX_EXPECT_EQUAL(gradient.getValueCount(), 2);

	gradient.removeIndex(0);
	IAUX_EXPECT_EQUAL(gradient.getValueCount(), 1);

	gradient.removeIndex(0);
	IAUX_EXPECT_EQUAL(gradient.getValueCount(), 0);
	IAUX_EXPECT_TRUE(gradient.isEmpty());
	IAUX_EXPECT_TRUE(gradient.getValues().empty());
}
