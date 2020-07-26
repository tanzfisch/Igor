#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/data/iaColor4.h>
using namespace iaux;

IAUX_TEST(Color4Tests, Initial1)
{
	iaColor4f color;

	IAUX_EXPECT_EQ(color._r, 0);
	IAUX_EXPECT_EQ(color._g, 0);
	IAUX_EXPECT_EQ(color._b, 0);
	IAUX_EXPECT_EQ(color._a, 0);
}

IAUX_TEST(Color4Tests, Initial2)
{
	iaColor4c color;

	IAUX_EXPECT_EQ(color._r, 0);
	IAUX_EXPECT_EQ(color._g, 0);
	IAUX_EXPECT_EQ(color._b, 0);
	IAUX_EXPECT_EQ(color._a, 0);
}

IAUX_TEST(Color4Tests, Initial1b)
{
	iaColor4f color(0.2, 0.7, 1.0, 0.4);

	IAUX_EXPECT_NEAR(color._r, 0.2, 0.0001);
	IAUX_EXPECT_NEAR(color._g, 0.7, 0.0001);
	IAUX_EXPECT_NEAR(color._b, 1.0, 0.0001);
	IAUX_EXPECT_NEAR(color._a, 0.4, 0.0001);
}

IAUX_TEST(Color4Tests, Initial2b)
{
	iaColor4c color(0, 128, 255, 60);

	IAUX_EXPECT_EQ(color._r, 0);
	IAUX_EXPECT_EQ(color._g, 128);
	IAUX_EXPECT_EQ(color._b, 255);
	IAUX_EXPECT_EQ(color._a, 60);
}

IAUX_TEST(Color4Tests, Set1)
{
	iaColor4f color;
	color.set(0.2, 0.7, 1.0, 0.4);

	IAUX_EXPECT_NEAR(color._r, 0.2, 0.0001);
	IAUX_EXPECT_NEAR(color._g, 0.7, 0.0001);
	IAUX_EXPECT_NEAR(color._b, 1.0, 0.0001);
	IAUX_EXPECT_NEAR(color._a, 0.4, 0.0001);
}

IAUX_TEST(Color4Tests, Set2)
{
	iaColor4c color;
	color.set(0, 128, 255, 60);

	IAUX_EXPECT_EQ(color._r, 0);
	IAUX_EXPECT_EQ(color._g, 128);
	IAUX_EXPECT_EQ(color._b, 255);
	IAUX_EXPECT_EQ(color._a, 60);
}

IAUX_TEST(Color4Tests, Assignment1)
{
	iaColor4f color;
	color = iaColor4f(0.2, 0.7, 1.0, 0.4);

	IAUX_EXPECT_NEAR(color._r, 0.2, 0.0001);
	IAUX_EXPECT_NEAR(color._g, 0.7, 0.0001);
	IAUX_EXPECT_NEAR(color._b, 1.0, 0.0001);
	IAUX_EXPECT_NEAR(color._a, 0.4, 0.0001);
}

IAUX_TEST(Color4Tests, Assignment2)
{
	iaColor4c color;
	color = iaColor4c(0, 128, 255, 60);

	IAUX_EXPECT_EQ(color._r, 0);
	IAUX_EXPECT_EQ(color._g, 128);
	IAUX_EXPECT_EQ(color._b, 255);
	IAUX_EXPECT_EQ(color._a, 60);
}

IAUX_TEST(Color4Tests, Add1)
{
	iaColor4f color(0.1, 0.3, 0, 0.1);
	color += iaColor4f(0.2, 0.7, 1.0, 0.3);

	IAUX_EXPECT_NEAR(color._r, 0.3, 0.0001);
	IAUX_EXPECT_NEAR(color._g, 1.0, 0.0001);
	IAUX_EXPECT_NEAR(color._b, 1.0, 0.0001);
	IAUX_EXPECT_NEAR(color._a, 0.4, 0.0001);
}

IAUX_TEST(Color4Tests, Add2)
{
	iaColor4c color(88, 128, 0, 30);
	color += iaColor4c(0, 127, 255, 30);

	IAUX_EXPECT_EQ(color._r, 88);
	IAUX_EXPECT_EQ(color._g, 255);
	IAUX_EXPECT_EQ(color._b, 255);
	IAUX_EXPECT_EQ(color._a, 60);
}

IAUX_TEST(Color4Tests, Sub1)
{
	iaColor4f color(0.1, 0.7, 0, 0.6);
	color -= iaColor4f(0.1, 0.3, 0, 0.2);

	IAUX_EXPECT_NEAR(color._r, 0.0, 0.0001);
	IAUX_EXPECT_NEAR(color._g, 0.4, 0.0001);
	IAUX_EXPECT_NEAR(color._b, 0.0, 0.0001);
	IAUX_EXPECT_NEAR(color._a, 0.4, 0.0001);
}

IAUX_TEST(Color4Tests, Sub2)
{
	iaColor4c color(88, 128, 255, 80);
	color -= iaColor4c(0, 127, 0, 20);

	IAUX_EXPECT_EQ(color._r, 88);
	IAUX_EXPECT_EQ(color._g, 1);
	IAUX_EXPECT_EQ(color._b, 255);
	IAUX_EXPECT_EQ(color._a, 60);
}

IAUX_TEST(Color4Tests, Scale1)
{
	iaColor4f color(0.1, 0.7, 0, 0.8);
	color *= 0.5;

	IAUX_EXPECT_NEAR(color._r, 0.05, 0.0001);
	IAUX_EXPECT_NEAR(color._g, 0.35, 0.0001);
	IAUX_EXPECT_NEAR(color._b, 0.0, 0.0001);
	IAUX_EXPECT_NEAR(color._a, 0.4, 0.0001);
}

IAUX_TEST(Color4Tests, Scale2)
{
	iaColor4c color(88, 128, 255, 120);
	color *= 0.5;

	IAUX_EXPECT_EQ(color._r, 44);
	IAUX_EXPECT_EQ(color._g, 64);
	IAUX_EXPECT_EQ(color._b, 127);
	IAUX_EXPECT_EQ(color._a, 60);
}

IAUX_TEST(Color4Tests, LERP1)
{
	iaColor4f color1(0.1, 0.12, 1.0, 0.2);
	iaColor4f color2(0.8, 0.7, 1.0, 0.6);

	iaColor4f color;
	color.lerp(color1, color2, 0.5);

	IAUX_EXPECT_NEAR(color._r, 0.45, 0.0001);
	IAUX_EXPECT_NEAR(color._g, 0.41, 0.0001);
	IAUX_EXPECT_NEAR(color._b, 1.0, 0.0001);
	IAUX_EXPECT_NEAR(color._a, 0.4, 0.0001);
}

IAUX_TEST(Color4Tests, LERP2)
{
	iaColor4c color1(0, 128, 200, 50);
	iaColor4c color2(84, 128, 180, 70);

	iaColor4c color;
	color.lerp(color1, color2, 0.5);

	IAUX_EXPECT_EQ(color._r, 42);
	IAUX_EXPECT_EQ(color._g, 128);
	IAUX_EXPECT_EQ(color._b, 190);
	IAUX_EXPECT_EQ(color._a, 60);
}
/*
IAUX_TEST(Color4Tests, RGBtoHSV1)
{
	iaColor4f rgba( 45 / 255, 128 / 255, 200 / 255, 0.2);
	iaColor4f color = convertRGBtoHSV(rgba);

	IAUX_EXPECT_NEAR(color._r, 208 / 255, 0.0001);
	IAUX_EXPECT_NEAR(color._g, 78 / 255, 0.0001);
	IAUX_EXPECT_NEAR(color._b, 78 / 255 , 0.0001);
	IAUX_EXPECT_NEAR(color._a, 0.2, 0.0001);
}

IAUX_TEST(Color4Tests, RGBtoHSV2)
{

}

/*
 static iaColor4<T> convertRGBtoHSV(iaColor4<T> &rgb)
        {
            iaColor4<T> result;

            bool maxR = false;
            bool maxG = false;
            bool maxB = false;
            T max = 0;

            if (rgb._r > rgb._g)
            {
                if (rgb._r > rgb._b)
                {
                    maxR = true;
                    max = rgb._r;
                }
                else
                {
                    maxB = true;
                    max = rgb._b;
                }
            }
            else
            {
                if (rgb._g > rgb._b)
                {
                    maxG = true;
                    max = rgb._g;
                }
                else
                {
                    maxB = true;
                    max = rgb._b;
                }
            }

            T min = std::min(std::min(rgb._r, rgb._g), rgb._b);
            T delta = max - min;

            if (delta == 0)
            {
                result._r = 0;
            }
            else if (maxR)
            {
                const float64 tmp = static_cast<float64>(((rgb._g - rgb._b) / delta));
                result._r = static_cast<T>(60.0 * fabs(fmod(tmp, 6.0)));
            }
            else if (maxG)
            {
                result._r = static_cast<T>(60.0 * (((rgb._b - rgb._r) / delta) + 2.0));
            }
            else if (maxB)
            {
                result._r = static_cast<T>(60.0 * (((rgb._r - rgb._g) / delta) + 4.0));
            }

            result._r /= 360.0;

            if (max == 0.0)
            {
                result._g = 0.0;
            }
            else
            {
                result._g = delta / max;
            }

            result._b = max;

            result._a = rgb._a;

            return result;
        }

*/
/*
IAUX_TEST(Color4Tests, HSVtoRGB1)
{
	iaColor4f rgba( 208 / 255, 78 / 255, 78 / 255, 0.2);
	iaColor4f color = convertRGBtoHSV(rgba);

	IAUX_EXPECT_NEAR(color._r, 45 / 255, 0.0001);
	IAUX_EXPECT_NEAR(color._g, 128 / 255, 0.0001);
	IAUX_EXPECT_NEAR(color._b, 200 / 255 , 0.0001);
	IAUX_EXPECT_NEAR(color._a, 0.2, 0.0001);
}

IAUX_TEST(Color4Tests, HSVtoRGB2)
{

}
*/