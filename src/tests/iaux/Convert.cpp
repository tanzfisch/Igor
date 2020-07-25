#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/data/iaConvert.h>
using namespace iaux;

IAUX_TEST(ConvertTests, RGBtoHSV)
{
	iaColor4c rgba(45, 128, 200, 50);
	iaColor4c hsva;
	iaConvert::convertRGBtoHSV(rgba, hsva);

	IAUX_EXPECT_EQ(hsva._r, 208);
	IAUX_EXPECT_EQ(hsva._g, 78);
	IAUX_EXPECT_EQ(hsva._b, 78);
	IAUX_EXPECT_EQ(hsva._a, 50);
}

IAUX_TEST(ConvertTests, HSVtoRGB)
{
	iaColor4c hsva(208, 78, 78, 50);
	iaColor4c rgba;
	iaConvert::convertHSVtoRGB(hsva, rgba);

	IAUX_EXPECT_EQ(rgba._r, 45);
	IAUX_EXPECT_EQ(rgba._g, 128);
	IAUX_EXPECT_EQ(rgba._b, 200);
	IAUX_EXPECT_EQ(rgba._a, 50);
}
