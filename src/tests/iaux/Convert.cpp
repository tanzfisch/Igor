#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/data/iaConvert.h>
using namespace iaux;

IAUX_TEST(ConvertTests, color3fto3c)
{
    iaColor3f color1(0.0, 0.5, 1.0);
    iaColor3c color2;
    iaConvert::convert(color1, color2);

    IAUX_EXPECT_EQUAL(color2._r, 0);
    IAUX_EXPECT_EQUAL(color2._g, 128);
    IAUX_EXPECT_EQUAL(color2._b, 255);

    iaConvert::convert(color2, color1);

    IAUX_EXPECT_EQUAL(color1._r, 0.0);
    IAUX_EXPECT_NEAR(color1._g, 0.5, 0.005);
    IAUX_EXPECT_EQUAL(color1._b, 1.0);
}

IAUX_TEST(ConvertTests, color4fto4c)
{
    iaColor4f color1(0.0, 0.5, 1.0, 0.5);
    iaColor4c color2;
    iaConvert::convert(color1, color2);

    IAUX_EXPECT_EQUAL(color2._r, 0);
    IAUX_EXPECT_EQUAL(color2._g, 128);
    IAUX_EXPECT_EQUAL(color2._b, 255);
    IAUX_EXPECT_EQUAL(color2._a, 128);

    iaConvert::convert(color2, color1);

    IAUX_EXPECT_EQUAL(color1._r, 0.0);
    IAUX_EXPECT_NEAR(color1._g, 0.5, 0.005);
    IAUX_EXPECT_EQUAL(color1._b, 1.0);
    IAUX_EXPECT_NEAR(color1._a, 0.5, 0.005);
}

IAUX_TEST(ConvertTests, RGBtoHSV3f)
{
    iaColor3f rgba(45 / 255.0, 128 / 255.0, 200 / 255.0);
    iaColor3f hsva;
    iaConvert::convertRGBtoHSV(rgba, hsva);

    IAUX_EXPECT_NEAR(hsva._r, 0.579387187, 0.01);
    IAUX_EXPECT_NEAR(hsva._g, 0.78, 0.01);
    IAUX_EXPECT_NEAR(hsva._b, 0.78, 0.01);
}

IAUX_TEST(ConvertTests, RGBtoHSV4f)
{
    iaColor4f rgba(45 / 255.0, 128 / 255.0, 200 / 255.0, 0.2);
    iaColor4f hsva;
    iaConvert::convertRGBtoHSV(rgba, hsva);

    IAUX_EXPECT_NEAR(hsva._r, 0.579387187, 0.005);
    IAUX_EXPECT_NEAR(hsva._g, 0.78, 0.01);
    IAUX_EXPECT_NEAR(hsva._b, 0.78, 0.01);
    IAUX_EXPECT_NEAR(hsva._a, 0.2, 0.005);
}

IAUX_TEST(ConvertTests, RGBtoHSV3c)
{
    iaColor3c rgb(45, 128, 200);
    iaColor3c hsv;
    iaConvert::convertRGBtoHSV(rgb, hsv);

    IAUX_EXPECT_EQUAL(hsv._r, 147);
    IAUX_EXPECT_EQUAL(hsv._g, 198); // expected rounding errors :-/
    IAUX_EXPECT_EQUAL(hsv._b, 200); // expected rounding errors :-/
}

IAUX_TEST(ConvertTests, RGBtoHSV4c)
{
    iaColor4c rgba(45, 128, 200, 50);
    iaColor4c hsva;
    iaConvert::convertRGBtoHSV(rgba, hsva);

    IAUX_EXPECT_EQUAL(hsva._r, 147);
    IAUX_EXPECT_EQUAL(hsva._g, 198); // expected rounding errors :-/
    IAUX_EXPECT_EQUAL(hsva._b, 200); // expected rounding errors :-/
    IAUX_EXPECT_EQUAL(hsva._a, 50);
}

IAUX_TEST(ConvertTests, HSVtoRGB3f)
{
    iaColor3f hsv(0.579387187, 0.78, 0.78);
    iaColor3f rgb;
    iaConvert::convertHSVtoRGB(hsv, rgb);

    IAUX_EXPECT_NEAR(rgb._r, 45 / 255.0, 0.005);
    IAUX_EXPECT_NEAR(rgb._g, 126 / 255.0, 0.005);
    IAUX_EXPECT_NEAR(rgb._b, 200 / 255.0, 0.005);
}

IAUX_TEST(ConvertTests, HSVtoRGB4f)
{
    iaColor4f hsva(0.579387187, 0.78, 0.78, 0.3);
    iaColor4f rgba;
    iaConvert::convertHSVtoRGB(hsva, rgba);

    IAUX_EXPECT_NEAR(rgba._r, 45 / 255.0, 0.005);
    IAUX_EXPECT_NEAR(rgba._g, 126 / 255.0, 0.005);
    IAUX_EXPECT_NEAR(rgba._b, 200 / 255.0, 0.005);
    IAUX_EXPECT_NEAR(rgba._a, 0.3, 0.005);
}

IAUX_TEST(ConvertTests, HSVtoRGB3c)
{
    iaColor3c hsva(147, 198, 200);
    iaColor3c rgba;
    iaConvert::convertHSVtoRGB(hsva, rgba);

    IAUX_EXPECT_EQUAL(rgba._r, 45);
    IAUX_EXPECT_EQUAL(rgba._g, 129); // expected rounding errors :-/
    IAUX_EXPECT_EQUAL(rgba._b, 200);
}

IAUX_TEST(ConvertTests, HSVtoRGB4c)
{
    iaColor4c hsva(147, 198, 200, 50);
    iaColor4c rgba;
    iaConvert::convertHSVtoRGB(hsva, rgba);

    IAUX_EXPECT_EQUAL(rgba._r, 45);
    IAUX_EXPECT_EQUAL(rgba._g, 129); // expected rounding errors :-/
    IAUX_EXPECT_EQUAL(rgba._b, 200);
    IAUX_EXPECT_EQUAL(rgba._a, 50);
}
