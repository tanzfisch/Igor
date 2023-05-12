#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/data/iaCircle.h>
using namespace iaux;

IAUX_TEST(CircleTests, Constructor1)
{
	iaCircled circle;

	IAUX_EXPECT_EQUAL(circle._center._x, 0.0);
	IAUX_EXPECT_EQUAL(circle._center._y, 0.0);
	IAUX_EXPECT_EQUAL(circle._radius, 1.0);
}

IAUX_TEST(CircleTests, Constructor2)
{
	iaCircled circle(1.0, 2.0, 3.0);

	IAUX_EXPECT_EQUAL(circle._center._x, 1.0);
	IAUX_EXPECT_EQUAL(circle._center._y, 2.0);
	IAUX_EXPECT_EQUAL(circle._radius, 3.0);
}

IAUX_TEST(CircleTests, Constructor3)
{
	iaCircled circle(iaVector2d(1.0, 2.0), 3.0);

	IAUX_EXPECT_EQUAL(circle._center._x, 1.0);
	IAUX_EXPECT_EQUAL(circle._center._y, 2.0);
	IAUX_EXPECT_EQUAL(circle._radius, 3.0);
}

IAUX_TEST(CircleTests, CopyConstructor)
{
	iaCircled circle1(iaVector2d(1.0, 2.0), 3.0);
	iaCircled circle2(circle1);

	IAUX_EXPECT_EQUAL(circle2._center._x, 1.0);
	IAUX_EXPECT_EQUAL(circle2._center._y, 2.0);
	IAUX_EXPECT_EQUAL(circle2._radius, 3.0);
}

IAUX_TEST(CircleTests, Getters)
{
	iaCircled circle(iaVector2d(1.0, 2.0), 3.0);

	IAUX_EXPECT_EQUAL(circle.getX(), 1.0);
	IAUX_EXPECT_EQUAL(circle.getY(), 2.0);
	IAUX_EXPECT_EQUAL(circle.getRadius(), 3.0);
	IAUX_EXPECT_EQUAL(circle.getCenter(), iaVector2d(1.0, 2.0));
}

IAUX_TEST(CircleTests, Setters)
{
	iaCircled circle;

	circle.setX(1.0);
	circle.setY(2.0);
	circle.setRadius(3.0);

	IAUX_EXPECT_EQUAL(circle._center._x, 1.0);
	IAUX_EXPECT_EQUAL(circle._center._y, 2.0);
	IAUX_EXPECT_EQUAL(circle._radius, 3.0);

	circle.setCenter(iaVector2d(5.0,6.0));

	IAUX_EXPECT_EQUAL(circle._center._x, 5.0);
	IAUX_EXPECT_EQUAL(circle._center._y, 6.0);

	circle.set(8.0,9.0,10.0);

	IAUX_EXPECT_EQUAL(circle._center._x, 8.0);
	IAUX_EXPECT_EQUAL(circle._center._y, 9.0);
	IAUX_EXPECT_EQUAL(circle._radius, 10.0);

	circle.set(iaVector2d(18.0,19.0),110.0);

	IAUX_EXPECT_EQUAL(circle._center._x, 18.0);
	IAUX_EXPECT_EQUAL(circle._center._y, 19.0);
	IAUX_EXPECT_EQUAL(circle._radius, 110.0);
}

IAUX_TEST(CircleTests, AssignmentOperator)
{
	iaCircled circle1(iaVector2d(1.0, 2.0), 3.0);
	iaCircled circle2 = circle1;

	IAUX_EXPECT_EQUAL(circle2._center._x, 1.0);
	IAUX_EXPECT_EQUAL(circle2._center._y, 2.0);
	IAUX_EXPECT_EQUAL(circle2._radius, 3.0);
}