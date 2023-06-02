#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/data/iaColor3.h>
using namespace iaux;

IAUX_TEST(Color3Tests, Initial1)
{
	iaColor3f color;

	IAUX_EXPECT_EQUAL(color._r, 0);
	IAUX_EXPECT_EQUAL(color._g, 0);
	IAUX_EXPECT_EQUAL(color._b, 0);
}

IAUX_TEST(Color3Tests, Initial2)
{
	iaColor3c color;

	IAUX_EXPECT_EQUAL(color._r, 0);
	IAUX_EXPECT_EQUAL(color._g, 0);
	IAUX_EXPECT_EQUAL(color._b, 0);
}

IAUX_TEST(Color3Tests, Initial1b)
{
	iaColor3f color(0.2, 0.7, 1.0);

	IAUX_EXPECT_NEAR(color._r, 0.2, 0.000001);
	IAUX_EXPECT_NEAR(color._g, 0.7, 0.000001);
	IAUX_EXPECT_NEAR(color._b, 1.0, 0.000001);
}

IAUX_TEST(Color3Tests, Initial2b)
{
	iaColor3c color(0, 128, 255);

	IAUX_EXPECT_EQUAL(color._r, 0);
	IAUX_EXPECT_EQUAL(color._g, 128);
	IAUX_EXPECT_EQUAL(color._b, 255);
}

IAUX_TEST(Color3Tests, Set1)
{
	iaColor3f color;
	color.set(0.2, 0.7, 1.0);

	IAUX_EXPECT_NEAR(color._r, 0.2, 0.000001);
	IAUX_EXPECT_NEAR(color._g, 0.7, 0.000001);
	IAUX_EXPECT_NEAR(color._b, 1.0, 0.000001);
}

IAUX_TEST(Color3Tests, Set2)
{
	iaColor3c color;
	color.set(0, 128, 255);

	IAUX_EXPECT_EQUAL(color._r, 0);
	IAUX_EXPECT_EQUAL(color._g, 128);
	IAUX_EXPECT_EQUAL(color._b, 255);
}

IAUX_TEST(Color3Tests, Assignment1)
{
	iaColor3f color;
	color = iaColor3f(0.2, 0.7, 1.0);

	IAUX_EXPECT_NEAR(color._r, 0.2, 0.000001);
	IAUX_EXPECT_NEAR(color._g, 0.7, 0.000001);
	IAUX_EXPECT_NEAR(color._b, 1.0, 0.000001);
}

IAUX_TEST(Color3Tests, Assignment2)
{
	iaColor3c color;
	color = iaColor3c(0, 128, 255);

	IAUX_EXPECT_EQUAL(color._r, 0);
	IAUX_EXPECT_EQUAL(color._g, 128);
	IAUX_EXPECT_EQUAL(color._b, 255);
}

IAUX_TEST(Color3Tests, Add1)
{
	iaColor3f color(0.1, 0.3, 0);
	color += iaColor3f(0.2, 0.7, 1.0);

	IAUX_EXPECT_NEAR(color._r, 0.3, 0.000001);
	IAUX_EXPECT_NEAR(color._g, 1.0, 0.000001);
	IAUX_EXPECT_NEAR(color._b, 1.0, 0.000001);
}

IAUX_TEST(Color3Tests, Add2)
{
	iaColor3c color(88, 128, 0);
	color += iaColor3c(0, 127, 255);

	IAUX_EXPECT_EQUAL(color._r, 88);
	IAUX_EXPECT_EQUAL(color._g, 255);
	IAUX_EXPECT_EQUAL(color._b, 255);
}

IAUX_TEST(Color3Tests, Sub1)
{
	iaColor3f color(0.1, 0.7, 0);
	color -= iaColor3f(0.1, 0.3, 0);

	IAUX_EXPECT_NEAR(color._r, 0.0, 0.000001);
	IAUX_EXPECT_NEAR(color._g, 0.4, 0.000001);
	IAUX_EXPECT_NEAR(color._b, 0, 0.000001);
}

IAUX_TEST(Color3Tests, Sub2)
{
	iaColor3c color(88, 128, 255);
	color -= iaColor3c(0, 127, 0);

	IAUX_EXPECT_EQUAL(color._r, 88);
	IAUX_EXPECT_EQUAL(color._g, 1);
	IAUX_EXPECT_EQUAL(color._b, 255);
}

IAUX_TEST(Color3Tests, Scale1)
{
	iaColor3f color(0.1, 0.7, 0);
	color *= 0.5;

	IAUX_EXPECT_NEAR(color._r, 0.05, 0.000001);
	IAUX_EXPECT_NEAR(color._g, 0.35, 0.000001);
	IAUX_EXPECT_NEAR(color._b, 0, 0.000001);
}

IAUX_TEST(Color3Tests, Scale2)
{
	iaColor3c color(88, 128, 255);
	color *= 0.5;

	IAUX_EXPECT_EQUAL(color._r, 44);
	IAUX_EXPECT_EQUAL(color._g, 64);
	IAUX_EXPECT_EQUAL(color._b, 127);
}