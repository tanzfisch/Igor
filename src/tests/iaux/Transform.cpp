#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/math/iaTransform.h>
using namespace iaux;

IAUX_TEST(TransformTests, Initial)
{
	iaTransformd trans;

	IAUX_EXPECT_EQUAL(trans._position, iaVector3d());
	IAUX_EXPECT_EQUAL(trans._scale, iaVector3d(1, 1, 1));
	IAUX_EXPECT_EQUAL(trans._orientation, iaQuaterniond(1, 0, 0, 0));

	IAUX_EXPECT_FALSE(trans.hasRotation());
	IAUX_EXPECT_FALSE(trans.hasScale());
	IAUX_EXPECT_FALSE(trans.hasTranslation());
}

IAUX_TEST(TransformTests, Compare)
{
	iaTransformd transA;
	iaTransformd transB;
	transB._orientation = iaQuaterniond::fromEuler(1, 2, 3);
	iaTransformd transC;
	transC._orientation = iaQuaterniond::fromEuler(1, 2, 3);

	IAUX_EXPECT_NOT_EQUAL(transA, transB);
	IAUX_EXPECT_EQUAL(transC, transB);
}

IAUX_TEST(TransformTests, getMatrix)
{
	iaVector3d translate(1, 2, 3);
	iaVector3d rotate(0.5, -0.2, 0.1);
	iaVector3d scale(1, 0.5, 1);
	iaTransformd transA(translate, iaQuaterniond::fromEuler(rotate), scale);

	iaMatrixd matrixA = transA.getMatrix();

	iaMatrixd matrixB;
	matrixB.translate(translate);
	matrixB.rotate(rotate);
	matrixB.scale(scale);

	IAUX_EXPECT_NEAR_MATRIX(matrixA, matrixB, 0.00001);
}

IAUX_TEST(TransformTests, Multiply)
{
	iaVector3d translateA(1.0, 2.0, -3.0);
	iaVector3d rotateA(0.0, 0.2, 0.2);
	iaVector3d scaleA(1.0, 1.0, 0.1);
	iaTransformd transA(translateA, iaQuaterniond::fromEuler(rotateA), scaleA);
	iaMatrixd matrixA = transA.getMatrix();

	iaVector3d translateB(-5.0, 4.0, -10.0);
	iaVector3d rotateB(0.0, 0.0, 0.0);
	iaVector3d scaleB(1.0, 0.1, 1.0);
	iaTransformd transB(translateB, iaQuaterniond::fromEuler(rotateB), scaleB);
	iaMatrixd matrixB = transB.getMatrix();

	iaTransformd transC = transA * transB;
	iaMatrixd matrixC = matrixA * matrixB;

	IAUX_EXPECT_NEAR_MATRIX(matrixC, transC.getMatrix(), 0.001);
}
