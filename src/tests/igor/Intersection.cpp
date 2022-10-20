#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <igor/data/iIntersection.h>
using namespace igor;

IAUX_TEST(IntersectionTests, SphereSphereIntersect)
{
    iSphered sphere1(iaVector3d(-13, 1, 0), 1);
    iSphered sphere2(iaVector3d(10, -2, 0), 1);
    iSphered sphere3(iaVector3d(5, 3, 0), 30);

    IAUX_EXPECT_FALSE(iIntersection::intersects(sphere1, sphere2));
    IAUX_EXPECT_TRUE(iIntersection::intersects(sphere1, sphere3));
    IAUX_EXPECT_TRUE(iIntersection::intersects(sphere3, sphere2));
}

IAUX_TEST(IntersectionTests, SphereSphereContains)
{
    iSphered sphere1(iaVector3d(-13, 1, 0), 1);
    iSphered sphere2(iaVector3d(10, -2, 0), 1);
    iSphered sphere3(iaVector3d(5, 3, 0), 30);

    IAUX_EXPECT_FALSE(iIntersection::contains(sphere1, sphere2));
    IAUX_EXPECT_FALSE(iIntersection::contains(sphere1, sphere3));
    IAUX_EXPECT_TRUE(iIntersection::contains(sphere3, sphere2));
}

IAUX_TEST(IntersectionTests, CircleRectangleContains)
{
    iaRectangled rectangle(-10, 30, 100, 100);

    iaCircled circle1(10, 10, 5);
    iaCircled circle2(-20, 10, 5);
    iaCircled circle3( 90, 50, 5);
    iaCircled circle4( 20, 130, 16);
    iaCircled circle5( 20, 50, 16);

    IAUX_EXPECT_FALSE(iIntersection::contains(rectangle, circle1));
    IAUX_EXPECT_FALSE(iIntersection::contains(rectangle, circle2));
    IAUX_EXPECT_FALSE(iIntersection::contains(rectangle, circle3));
    IAUX_EXPECT_FALSE(iIntersection::contains(rectangle, circle4));
    IAUX_EXPECT_TRUE(iIntersection::contains(rectangle, circle5));
}

IAUX_TEST(IntersectionTests, SphereInFrontOfPlane)
{
    iSphered sphere1(iaVector3d(-20, 0, 0), 1);
    iSphered sphere2(iaVector3d(20, 0, 0), 1);
    iSphered sphere3(iaVector3d(0, 0, 0), 1);
    iSphered sphere4(iaVector3d(-11, 0, 0), 2);
    iSphered sphere5(iaVector3d(11, 0, 0), 2);
    iSphered sphere6(iaVector3d(-9, 0, 0), 2);
    iSphered sphere7(iaVector3d(9, 0, 0), 2);

    iPlaned plane1(iaVector3d(1, 0, 0), -10);
    iPlaned plane2(iaVector3d(-1, 0, 0), -10);
    iPlaned plane3(iaVector3d(1, 0, 0), 10);
    iPlaned plane4(iaVector3d(-1, 0, 0), 10);

    IAUX_EXPECT_FALSE(iIntersection::inFrontOf(sphere1, plane1));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(sphere2, plane1));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(sphere3, plane1));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(sphere4, plane1));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(sphere5, plane1));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(sphere6, plane1));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(sphere7, plane1));

    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(sphere1, plane2));
    IAUX_EXPECT_FALSE(iIntersection::inFrontOf(sphere2, plane2));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(sphere3, plane2));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(sphere4, plane2));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(sphere5, plane2));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(sphere6, plane2));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(sphere7, plane2));

    IAUX_EXPECT_FALSE(iIntersection::inFrontOf(sphere1, plane3));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(sphere2, plane3));
    IAUX_EXPECT_FALSE(iIntersection::inFrontOf(sphere3, plane3));
    IAUX_EXPECT_FALSE(iIntersection::inFrontOf(sphere4, plane3));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(sphere5, plane3));
    IAUX_EXPECT_FALSE(iIntersection::inFrontOf(sphere6, plane3));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(sphere7, plane3));

    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(sphere1, plane4));
    IAUX_EXPECT_FALSE(iIntersection::inFrontOf(sphere2, plane4));
    IAUX_EXPECT_FALSE(iIntersection::inFrontOf(sphere3, plane4));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(sphere4, plane4));
    IAUX_EXPECT_FALSE(iIntersection::inFrontOf(sphere5, plane4));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(sphere6, plane4));
    IAUX_EXPECT_FALSE(iIntersection::inFrontOf(sphere7, plane4));
}

IAUX_TEST(IntersectionTests, SphereIntersectsFrustum)
{
    iSphered sphere1(iaVector3d(0, 0, 0), 1);
    iSphered sphere2(iaVector3d(0, 0, -20), 1);
    iSphered sphere3(iaVector3d(0, 0, -9), 2);
    iSphered sphere4(iaVector3d(0, 0, -11), 2);
    iSphered sphere5(iaVector3d(0, 0, -200), 90);
    iSphered sphere6(iaVector3d(0, 0, -99), 2);
    iSphered sphere7(iaVector3d(0, 0, -101), 2);
    iSphered sphere8(iaVector3d(-60, 0, -50), 2);

    iaMatrixd matrix;
    matrix.perspective(90, 1, 10, 100);

    iFrustumd frustum(matrix);

    IAUX_EXPECT_FALSE(iIntersection::intersects(sphere1, frustum));
    IAUX_EXPECT_TRUE(iIntersection::intersects(sphere2, frustum));
    IAUX_EXPECT_TRUE(iIntersection::intersects(sphere3, frustum));
    IAUX_EXPECT_TRUE(iIntersection::intersects(sphere4, frustum));
    IAUX_EXPECT_FALSE(iIntersection::intersects(sphere5, frustum));
    IAUX_EXPECT_TRUE(iIntersection::intersects(sphere6, frustum));
    IAUX_EXPECT_TRUE(iIntersection::intersects(sphere7, frustum));
    IAUX_EXPECT_FALSE(iIntersection::intersects(sphere8, frustum));
}

IAUX_TEST(IntersectionTests, CubeInFrontOfPlane)
{
    iAACubed cube1(iaVector3d(-20, 0, 0), 1);
    iAACubed cube2(iaVector3d(20, 0, 0), 1);
    iAACubed cube3(iaVector3d(0, 0, 0), 1);
    iAACubed cube4(iaVector3d(-11, 0, 0), 2);
    iAACubed cube5(iaVector3d(11, 0, 0), 2);
    iAACubed cube6(iaVector3d(-9, 0, 0), 2);
    iAACubed cube7(iaVector3d(9, 0, 0), 2);

    iPlaned plane1(iaVector3d(1, 0, 0), -10);
    iPlaned plane2(iaVector3d(-1, 0, 0), -10);
    iPlaned plane3(iaVector3d(1, 0, 0), 10);
    iPlaned plane4(iaVector3d(-1, 0, 0), 10);

    IAUX_EXPECT_FALSE(iIntersection::inFrontOf(cube1, plane1));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(cube2, plane1));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(cube3, plane1));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(cube4, plane1));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(cube5, plane1));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(cube6, plane1));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(cube7, plane1));

    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(cube1, plane2));
    IAUX_EXPECT_FALSE(iIntersection::inFrontOf(cube2, plane2));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(cube3, plane2));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(cube4, plane2));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(cube5, plane2));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(cube6, plane2));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(cube7, plane2));

    IAUX_EXPECT_FALSE(iIntersection::inFrontOf(cube1, plane3));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(cube2, plane3));
    IAUX_EXPECT_FALSE(iIntersection::inFrontOf(cube3, plane3));
    IAUX_EXPECT_FALSE(iIntersection::inFrontOf(cube4, plane3));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(cube5, plane3));
    IAUX_EXPECT_FALSE(iIntersection::inFrontOf(cube6, plane3));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(cube7, plane3));

    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(cube1, plane4));
    IAUX_EXPECT_FALSE(iIntersection::inFrontOf(cube2, plane4));
    IAUX_EXPECT_FALSE(iIntersection::inFrontOf(cube3, plane4));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(cube4, plane4));
    IAUX_EXPECT_FALSE(iIntersection::inFrontOf(cube5, plane4));
    IAUX_EXPECT_TRUE(iIntersection::inFrontOf(cube6, plane4));
    IAUX_EXPECT_FALSE(iIntersection::inFrontOf(cube7, plane4));
}