#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/math/iaMatrix.h>
#include <iaux/math/iaRandomNumberGenerator.h>
using namespace iaux;

#include <cstring>

static const float64 identityMatrix[] =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1 
};

static const float32 identityMatrixFloat32[] =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

IAUX_TEST(MatrixTests, Initial) 
{
	iaMatrixd matrix;

	IAUX_EXPECT_TRUE(0 == std::memcmp(matrix.getData(), identityMatrix, sizeof(matrix)));
}

IAUX_TEST(MatrixTests, Convert)
{
	iaMatrixd matrix;
	iaMatrixf matrix2 = matrix.convert<float32>();
	
	IAUX_EXPECT_TRUE(0 == std::memcmp(matrix2.getData(), identityMatrixFloat32, sizeof(matrix2)));
}

IAUX_TEST(MatrixTests, Identity) 
{
	iaMatrixd matrix;
	matrix.identity();

	IAUX_EXPECT_TRUE(0 == std::memcmp(matrix.getData(), identityMatrix, sizeof(matrix)));
}

IAUX_TEST(MatrixTests, Translate)
{
	static const float64 translateMatrix[] =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		1, 2, 3, 1
	};

	iaMatrixd matrix;
	matrix.translate(1, 2, 3);

	IAUX_EXPECT_TRUE(0 == std::memcmp(matrix.getData(), translateMatrix, sizeof(matrix)));
}

IAUX_TEST(MatrixTests, TranslateNegative)
{
	static const float64 translateNegativeMatrix[] =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-1, -2, -3, 1
	};

	iaMatrixd matrix;
	matrix.translate(-1, -2, -3);

	IAUX_EXPECT_TRUE(0 == std::memcmp(matrix.getData(), translateNegativeMatrix, sizeof(matrix)));
}

IAUX_TEST(MatrixTests, Scale)
{
	static const float64 scaleMatrix[] =
	{
		10, 0, 0, 0,
		0, 20, 0, 0,
		0, 0, 30, 0,
		0, 0, 0, 1
	};


	iaMatrixd matrix;
	matrix.scale(10, 20, 30);

	IAUX_EXPECT_TRUE(0 == std::memcmp(matrix.getData(), scaleMatrix, sizeof(matrix)));
}

IAUX_TEST(MatrixTests, ScaleNegative)
{
	static const float64 scaleNegativeMatrix[] =
	{
		-10, 0, 0, 0,
		0, -20, 0, 0,
		0, 0, -30, 0,
		0, 0, 0, 1
	};

	iaMatrixd matrix;
	matrix.scale(-10, -20, -30);

	IAUX_EXPECT_TRUE(0 == std::memcmp(matrix.getData(), scaleNegativeMatrix, sizeof(matrix)));
}

IAUX_TEST(MatrixTests, DecomposeTranslate)
{
	iaMatrixd matrix;
	matrix.translate(13, 12, 453);

	iaVector3d scale;
	iaQuaterniond orientation;
	iaVector3d translate;
	iaVector3d shear;
	iaVector4d perspective;

	matrix.decompose(scale, orientation, translate, shear, perspective);

	IAUX_EXPECT_EQUAL(translate._x, 13);
	IAUX_EXPECT_EQUAL(translate._y, 12);
	IAUX_EXPECT_EQUAL(translate._z, 453);
}

IAUX_TEST(MatrixTests, DecomposeTranslateNegative)
{
	iaMatrixd matrix;
	matrix.translate(13, -12, -453);

	iaVector3d scale;
	iaQuaterniond orientation;
	iaVector3d translate;
	iaVector3d shear;
	iaVector4d perspective;

	matrix.decompose(scale, orientation, translate, shear, perspective);

	IAUX_EXPECT_EQUAL(translate._x, 13);
	IAUX_EXPECT_EQUAL(translate._y, -12);
	IAUX_EXPECT_EQUAL(translate._z, -453);
}

IAUX_TEST(MatrixTests, Transpose)
{
	iaMatrixd matrix;

	matrix[1] = 10;
	matrix[2] = 20;
	matrix[3] = 30;

	matrix[4] = 40;
	matrix[8] = 80;
	matrix[12] = 120;

	matrix[10] = 5;


	matrix.transpose();

	static const float64 cmpMatrix[] =
	{
		1, 40, 80, 120,
		10, 1, 0, 0,
		20, 0, 5, 0,
		30, 0, 0, 1
	};
	
	IAUX_EXPECT_TRUE(0 == std::memcmp(matrix.getData(), cmpMatrix, sizeof(matrix)));
}

// tocalc determinants https://matrix.reshish.com/detCalculation.php
IAUX_TEST(MatrixTests, DeterminantOfID)
{
	iaMatrixd matrix;

	IAUX_EXPECT_NEAR(matrix.determinant(), 1, 0.00000001);
}

IAUX_TEST(MatrixTests, DeterminantOfTranslation)
{
	iaMatrixd matrix;
	matrix.translate(1,2,-3);

	IAUX_EXPECT_NEAR(matrix.determinant(), 1, 0.00000001);
}

IAUX_TEST(MatrixTests, DeterminantOfScale)
{
	iaMatrixd matrix;
	matrix.scale(2, -3, 5);

	IAUX_EXPECT_NEAR(matrix.determinant(), -30, 0.00000001);
}

IAUX_TEST(MatrixTests, DeterminantOfRotate)
{
	iaMatrixd matrix;
	matrix.rotate(0.1, -0.2, 0.6);

	IAUX_EXPECT_NEAR(matrix.determinant(), 1.0, 0.00000001);
}

IAUX_TEST(MatrixTests, DeterminantOfShear)
{
	iaMatrixd matrix;
	matrix.shear(0.1, -0.2, 0.6);

	IAUX_EXPECT_NEAR(matrix.determinant(), 1.0, 0.00000001);
}

IAUX_TEST(MatrixTests, DeterminantOfPerspective)
{
	iaMatrixd matrix;
	matrix.perspective(45, 0.25, 0.1, 1000);

	IAUX_EXPECT_NEAR(matrix.determinant(), -4.6632078945301687105, 0.00001);
}

IAUX_TEST(MatrixTests, DeterminantOfMixedMatrix)
{
	iaMatrixd matrix;
	matrix.translate(1, 2, -3);
	matrix.scale(2, -3, 5);
	matrix.rotate(0.1, -0.2, 0.6);
	matrix.shear(0.1, -0.2, 0.6);

	IAUX_EXPECT_NEAR(matrix.determinant(), -30, 0.00000001);
}

IAUX_TEST(MatrixTests, DecomposeScale)
{
	iaMatrixd matrix;
	matrix.scale(1, 2, 0.003);

	iaVector3d scale;
	iaQuaterniond orientation;
	iaVector3d translate;
	iaVector3d shear;
	iaVector4d perspective;

	matrix.decompose(scale, orientation, translate, shear, perspective);

	IAUX_EXPECT_EQUAL(scale._x, 1);
	IAUX_EXPECT_EQUAL(scale._y, 2);
	IAUX_EXPECT_EQUAL(scale._z, 0.003);
}

IAUX_TEST(MatrixTests, DecomposeScaleNegative)
{
	iaMatrixd matrix;
	matrix.scale(-1, 2, 0.003);

	iaVector3d scale;
	iaQuaterniond orientation;
	iaVector3d translate;
	iaVector3d shear;
	iaVector4d perspective;

	matrix.decompose(scale, orientation, translate, shear, perspective);

	IAUX_EXPECT_EQUAL(scale._x, -1);
	IAUX_EXPECT_EQUAL(scale._y, 2);
	IAUX_EXPECT_EQUAL(scale._z, 0.003);
}

IAUX_TEST(MatrixTests, DecomposeShear)
{
	iaMatrixd matrix;
	matrix.shear(5, 0.17,-12);

	iaVector3d scale;
	iaQuaterniond orientation;
	iaVector3d translate;
	iaVector3d shear;
	iaVector4d perspective;

	matrix.decompose(scale, orientation, translate, shear, perspective);

	IAUX_EXPECT_EQUAL(shear._x, 5);
	IAUX_EXPECT_EQUAL(shear._y, 0.17);
	IAUX_EXPECT_EQUAL(shear._z, -12);
}

IAUX_TEST(MatrixTests, DecomposeRotateX)
{
	iaMatrixd matrix;
	matrix.rotate(0.5, iaAxis::X);

	iaVector3d scale;
	iaQuaterniond orientation;
	iaVector3d translate;
	iaVector3d shear;
	iaVector4d perspective;

	matrix.decompose(scale, orientation, translate, shear, perspective);

	iaVector3d rotate;
	orientation.getEuler(rotate);

	IAUX_EXPECT_NEAR(rotate._x, 0.5, 0.0000001);
	IAUX_EXPECT_NEAR(rotate._y, 0, 0.0000001);
	IAUX_EXPECT_NEAR(rotate._z, 0, 0.0000001);
}

IAUX_TEST(MatrixTests, DecomposeRotateY)
{
	iaMatrixd matrix;
	matrix.rotate(-0.25, iaAxis::Y);

	iaVector3d scale;
	iaQuaterniond orientation;
	iaVector3d translate;
	iaVector3d shear;
	iaVector4d perspective;

	matrix.decompose(scale, orientation, translate, shear, perspective);

	iaVector3d rotate;
	orientation.getEuler(rotate);

	IAUX_EXPECT_NEAR(rotate._x, 0, 0.0000001);
	IAUX_EXPECT_NEAR(rotate._y, -0.25, 0.0000001);
	IAUX_EXPECT_NEAR(rotate._z, 0, 0.0000001);
}

IAUX_TEST(MatrixTests, DecomposeRotateZ)
{
	iaMatrixd matrix;
	matrix.rotate(0.333, iaAxis::Z);

	iaVector3d scale;
	iaQuaterniond orientation;
	iaVector3d translate;
	iaVector3d shear;
	iaVector4d perspective;

	matrix.decompose(scale, orientation, translate, shear, perspective);

	iaVector3d rotate;
	orientation.getEuler(rotate);

	IAUX_EXPECT_NEAR(rotate._x, 0, 0.0000001);
	IAUX_EXPECT_NEAR(rotate._y, 0, 0.0000001);
	IAUX_EXPECT_NEAR(rotate._z, 0.333, 0.0000001);
}

IAUX_TEST(MatrixTests, DecomposeRotate100)
{
	iaRandomNumberGeneratoru random(1337);

	for (int i = 0; i < 100; ++i)
	{
		iaMatrixd matrix;
		iaVector3d rotation((random.getNext() % 2000) / 1000.0 - 1.0 * M_PI_2, 
			(random.getNext() % 2000) / 1000.0 - 1.0 * M_PI_2, 
			(random.getNext() % 2000) / 1000.0 - 1.0 * M_PI_2);
		matrix.rotate(rotation);

		iaVector3d scale;
		iaQuaterniond orientation;
		iaVector3d translate;
		iaVector3d shear;
		iaVector4d perspective;

		matrix.decompose(scale, orientation, translate, shear, perspective);

		iaVector3d rotate;
		orientation.getEuler(rotate);

		IAUX_EXPECT_NEAR(rotate._x, rotation._x, 0.0000001);
		IAUX_EXPECT_NEAR(rotate._y, rotation._y, 0.0000001);
		IAUX_EXPECT_NEAR(rotate._z, rotation._z, 0.0000001);
	}
}

// todo #235
IAUX_TEST(MatrixTests, DecomposeRecompose)
{
	iaMatrixd matrix;
	matrix.rotate(0.5 * M_PI, 0, 0);
	matrix.scale(1, 2, 1);
	matrix.translate(10, 0, -7);

	iaVector3d scale;
	iaQuaterniond orientation;
	iaVector3d translate;
	iaVector3d shear;
	iaVector4d perspective;

	matrix.decompose(scale, orientation, translate, shear, perspective);

	iaMatrixd matrix2;
	matrix2.recompose(scale, orientation, translate, shear, perspective);

	IAUX_EXPECT_NEAR_MATRIX(matrix, matrix2, 0.00001);
}