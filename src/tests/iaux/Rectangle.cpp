#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/data/iaRectangle.h>
using namespace iaux;

IAUX_TEST(RectangleTests, Constructor1)
{
	iaRectanglei rectangle;

	IAUX_EXPECT_EQUAL(rectangle._x, 0);
	IAUX_EXPECT_EQUAL(rectangle._y, 0);
	IAUX_EXPECT_EQUAL(rectangle._width, 0);
	IAUX_EXPECT_EQUAL(rectangle._height, 0);
}

IAUX_TEST(RectangleTests, Constructor2)
{
	iaRectanglei rectangle(1, 2);

	IAUX_EXPECT_EQUAL(rectangle._x, 1);
	IAUX_EXPECT_EQUAL(rectangle._y, 2);
	IAUX_EXPECT_EQUAL(rectangle._width, 0);
	IAUX_EXPECT_EQUAL(rectangle._height, 0);
}

IAUX_TEST(RectangleTests, Constructor3)
{
	iaRectanglei rectangle(1, 2, 3, 4);

	IAUX_EXPECT_EQUAL(rectangle._x, 1);
	IAUX_EXPECT_EQUAL(rectangle._y, 2);
	IAUX_EXPECT_EQUAL(rectangle._width, 3);
	IAUX_EXPECT_EQUAL(rectangle._height, 4);
}

IAUX_TEST(RectangleTests, Constructor4)
{
	iaRectanglei rectangle(iaVector2i(1, 2), iaVector2i(3, 4));

	IAUX_EXPECT_EQUAL(rectangle._x, 1);
	IAUX_EXPECT_EQUAL(rectangle._y, 2);
	IAUX_EXPECT_EQUAL(rectangle._width, 3);
	IAUX_EXPECT_EQUAL(rectangle._height, 4);
}

IAUX_TEST(RectangleTests, CopyConstructor)
{
	iaRectanglei rectangle1(iaVector2i(1, 2), iaVector2i(3, 4));
	iaRectanglei rectangle2(rectangle1);

	IAUX_EXPECT_EQUAL(rectangle2._x, 1);
	IAUX_EXPECT_EQUAL(rectangle2._y, 2);
	IAUX_EXPECT_EQUAL(rectangle2._width, 3);
	IAUX_EXPECT_EQUAL(rectangle2._height, 4);
}

IAUX_TEST(RectangleTests, adjust)
{
	iaRectanglei rectangle(10, 20, 30, 40);

	rectangle.adjust(1, 2, -3, 4);

	IAUX_EXPECT_EQUAL(rectangle._x, 11);
	IAUX_EXPECT_EQUAL(rectangle._y, 22);
	IAUX_EXPECT_EQUAL(rectangle._width, 27);
	IAUX_EXPECT_EQUAL(rectangle._height, 44);
}

IAUX_TEST(RectangleTests, Getter)
{
	iaRectanglei rectangle(10, 20, 30, 40);

	IAUX_EXPECT_EQUAL(rectangle.getX(), 10);
	IAUX_EXPECT_EQUAL(rectangle.getY(), 20);
	IAUX_EXPECT_EQUAL(rectangle.getWidth(), 30);
	IAUX_EXPECT_EQUAL(rectangle.getHeight(), 40);

	IAUX_EXPECT_EQUAL(rectangle.getLeft(), 10);
	IAUX_EXPECT_EQUAL(rectangle.getRight(), 10 + 30);
	IAUX_EXPECT_EQUAL(rectangle.getTop(), 20);
	IAUX_EXPECT_EQUAL(rectangle.getBottom(), 20 + 40);

	IAUX_EXPECT_EQUAL(rectangle.getCenter(), iaVector2i(25, 40));
	IAUX_EXPECT_EQUAL(rectangle.getTopLeft(), iaVector2i(10, 20));
	IAUX_EXPECT_EQUAL(rectangle.getTopRight(), iaVector2i(10 + 30, 20));
	IAUX_EXPECT_EQUAL(rectangle.getBottomLeft(), iaVector2i(10, 20 + 40));
	IAUX_EXPECT_EQUAL(rectangle.getBottomRight(), iaVector2i(10 + 30, 20 + 40));
}

IAUX_TEST(RectangleTests, SetCenter)
{
	iaRectanglei rectangle(10, 20, 30, 40);

	rectangle.setCenter(33, 44);

	IAUX_EXPECT_EQUAL(rectangle.getCenter(), iaVector2i(33, 44));
	IAUX_EXPECT_EQUAL(rectangle.getX(), 18);
	IAUX_EXPECT_EQUAL(rectangle.getY(), 24);
	IAUX_EXPECT_EQUAL(rectangle.getWidth(), 30);
	IAUX_EXPECT_EQUAL(rectangle.getHeight(), 40);

	rectangle.setCenter(iaVector2i(133, 144));

	IAUX_EXPECT_EQUAL(rectangle.getCenter(), iaVector2i(133, 144));
	IAUX_EXPECT_EQUAL(rectangle.getX(), 118);
	IAUX_EXPECT_EQUAL(rectangle.getY(), 124);
	IAUX_EXPECT_EQUAL(rectangle.getWidth(), 30);
	IAUX_EXPECT_EQUAL(rectangle.getHeight(), 40);
}

IAUX_TEST(RectangleTests, Setter)
{
	iaRectanglei rectangle;

	rectangle.setSize(100, 200);

	IAUX_EXPECT_EQUAL(rectangle.getX(), 0);
	IAUX_EXPECT_EQUAL(rectangle.getY(), 0);
	IAUX_EXPECT_EQUAL(rectangle.getWidth(), 100);
	IAUX_EXPECT_EQUAL(rectangle.getHeight(), 200);

	rectangle.setTopLeft(10, 20);

	IAUX_EXPECT_EQUAL(rectangle.getX(), 10);
	IAUX_EXPECT_EQUAL(rectangle.getY(), 20);
	IAUX_EXPECT_EQUAL(rectangle.getWidth(), 100);
	IAUX_EXPECT_EQUAL(rectangle.getHeight(), 200);

	rectangle.setX(1);
	rectangle.setY(2);
	rectangle.setWidth(3);
	rectangle.setHeight(4);

	IAUX_EXPECT_EQUAL(rectangle.getX(), 1);
	IAUX_EXPECT_EQUAL(rectangle.getY(), 2);
	IAUX_EXPECT_EQUAL(rectangle.getWidth(), 3);
	IAUX_EXPECT_EQUAL(rectangle.getHeight(), 4);	

	rectangle.set(5,6,7,8);

	IAUX_EXPECT_EQUAL(rectangle.getX(), 5);
	IAUX_EXPECT_EQUAL(rectangle.getY(), 6);
	IAUX_EXPECT_EQUAL(rectangle.getWidth(), 7);
	IAUX_EXPECT_EQUAL(rectangle.getHeight(), 8);		
}

IAUX_TEST(RectangleTests, AssignmentOperator)
{
	iaRectanglei rectangle1(iaVector2i(1, 2), iaVector2i(3, 4));
	iaRectanglei rectangle2 = rectangle1;

	IAUX_EXPECT_EQUAL(rectangle2._x, 1);
	IAUX_EXPECT_EQUAL(rectangle2._y, 2);
	IAUX_EXPECT_EQUAL(rectangle2._width, 3);
	IAUX_EXPECT_EQUAL(rectangle2._height, 4);	
}

IAUX_TEST(RectangleTests, CompareRectangles)
{
	iaRectanglei rectangle1(1,2,3,4);
	iaRectanglei rectangle2(1,2,3,4);
	iaRectanglei rectangle3(6,9,4,2);

	IAUX_EXPECT_EQUAL(rectangle1, rectangle2);
	IAUX_EXPECT_NOT_EQUAL(rectangle1, rectangle3);
}