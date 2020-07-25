#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/data/iaConvert.h>
using namespace iaux;

IAUX_TEST(ConvertTests, color3fto3c)
{
	iaColor3f color1(0.0, 0.5, 1.0);
	iaColor3c color2;
	iaConvert::convert(color1, color2);

	IAUX_EXPECT_EQ(color2._r, 0);
	IAUX_EXPECT_EQ(color2._g, 128);
	IAUX_EXPECT_EQ(color2._b, 255);

	iaConvert::convert(color2, color1);

	IAUX_EXPECT_EQ(color1._r, 0.0);
	IAUX_EXPECT_NEAR(color1._g, 0.5, 0.005);
	IAUX_EXPECT_EQ(color1._b, 1.0);
}

IAUX_TEST(ConvertTests, color4fto4c)
{
	iaColor4f color1(0.0, 0.5, 1.0, 0.5);
	iaColor4c color2;
	iaConvert::convert(color1, color2);

	IAUX_EXPECT_EQ(color2._r, 0);
	IAUX_EXPECT_EQ(color2._g, 128);
	IAUX_EXPECT_EQ(color2._b, 255);
	IAUX_EXPECT_EQ(color2._a, 128);

	iaConvert::convert(color2, color1);

	IAUX_EXPECT_EQ(color1._r, 0.0);
	IAUX_EXPECT_NEAR(color1._g, 0.5, 0.005);
	IAUX_EXPECT_EQ(color1._b, 1.0);
	IAUX_EXPECT_NEAR(color1._a, 0.5, 0.005);
}

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
