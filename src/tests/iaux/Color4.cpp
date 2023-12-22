#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/data/iaColor4.h>
using namespace iaux;

IAUX_TEST(Color4Tests, Initial1)
{
	iaColor4f color;

	IAUX_EXPECT_EQUAL(color._r, 1.0);
	IAUX_EXPECT_EQUAL(color._g, 1.0);
	IAUX_EXPECT_EQUAL(color._b, 1.0);
	IAUX_EXPECT_EQUAL(color._a, 1.0);
}

IAUX_TEST(Color4Tests, Initial2)
{
	iaColor4c color;

	IAUX_EXPECT_EQUAL(color._r, 0xff);
	IAUX_EXPECT_EQUAL(color._g, 0xff);
	IAUX_EXPECT_EQUAL(color._b, 0xff);
	IAUX_EXPECT_EQUAL(color._a, 0xff);
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

	IAUX_EXPECT_EQUAL(color._r, 0);
	IAUX_EXPECT_EQUAL(color._g, 128);
	IAUX_EXPECT_EQUAL(color._b, 255);
	IAUX_EXPECT_EQUAL(color._a, 60);
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

	IAUX_EXPECT_EQUAL(color._r, 0);
	IAUX_EXPECT_EQUAL(color._g, 128);
	IAUX_EXPECT_EQUAL(color._b, 255);
	IAUX_EXPECT_EQUAL(color._a, 60);
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

	IAUX_EXPECT_EQUAL(color._r, 0);
	IAUX_EXPECT_EQUAL(color._g, 128);
	IAUX_EXPECT_EQUAL(color._b, 255);
	IAUX_EXPECT_EQUAL(color._a, 60);
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

	IAUX_EXPECT_EQUAL(color._r, 88);
	IAUX_EXPECT_EQUAL(color._g, 255);
	IAUX_EXPECT_EQUAL(color._b, 255);
	IAUX_EXPECT_EQUAL(color._a, 60);
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

	IAUX_EXPECT_EQUAL(color._r, 88);
	IAUX_EXPECT_EQUAL(color._g, 1);
	IAUX_EXPECT_EQUAL(color._b, 255);
	IAUX_EXPECT_EQUAL(color._a, 60);
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

	IAUX_EXPECT_EQUAL(color._r, 44);
	IAUX_EXPECT_EQUAL(color._g, 64);
	IAUX_EXPECT_EQUAL(color._b, 127);
	IAUX_EXPECT_EQUAL(color._a, 60);
}
