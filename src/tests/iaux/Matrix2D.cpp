#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/math/iaMatrix2D.h>
using namespace iaux;

#include <cstring>

#define COMP_MATRIX(m1,m2) \
	IAUX_EXPECT_NEAR(m1._m[0], m2._m[0], 0.0000001); \
	IAUX_EXPECT_NEAR(m1._m[1], m2._m[1], 0.0000001); \
	IAUX_EXPECT_NEAR(m1._m[2], m2._m[2], 0.0000001); \
	IAUX_EXPECT_NEAR(m1._m[3], m2._m[3], 0.0000001); \
	IAUX_EXPECT_NEAR(m1._m[4], m2._m[4], 0.0000001); \
	IAUX_EXPECT_NEAR(m1._m[5], m2._m[5], 0.0000001); \
	IAUX_EXPECT_NEAR(m1._m[6], m2._m[6], 0.0000001); \
	IAUX_EXPECT_NEAR(m1._m[7], m2._m[7], 0.0000001); \
	IAUX_EXPECT_NEAR(m1._m[8], m2._m[8], 0.0000001)

static const float64 identityMatrix[] =
{
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0
};

IAUX_TEST(Matrix3x3Tests, Initial)
{
	iaMatrix3x3d matrix;

	IAUX_EXPECT_EQ(matrix[0], identityMatrix[0]);
	IAUX_EXPECT_EQ(matrix[1], identityMatrix[1]);
	IAUX_EXPECT_EQ(matrix[2], identityMatrix[2]);

	IAUX_EXPECT_EQ(matrix[3], identityMatrix[3]);
	IAUX_EXPECT_EQ(matrix[4], identityMatrix[4]);
	IAUX_EXPECT_EQ(matrix[5], identityMatrix[5]);

	IAUX_EXPECT_EQ(matrix[6], identityMatrix[6]);
	IAUX_EXPECT_EQ(matrix[7], identityMatrix[7]);
	IAUX_EXPECT_EQ(matrix[8], identityMatrix[8]);
}

IAUX_TEST(Matrix3x3Tests, Convert)
{
	iaMatrix3x3d matrix;
	iaMatrix3x3f matrix2 = matrix.convert<float32>();

	COMP_MATRIX(matrix, matrix2);
}

IAUX_TEST(Matrix3x3Tests, Identity)
{
	iaMatrix3x3d matrix;
	matrix.identity();

	IAUX_EXPECT_TRUE(0 == std::memcmp(matrix.getData(), identityMatrix, sizeof(matrix)));
}

IAUX_TEST(Matrix3x3Tests, Translate)
{
	static const float64 translateMatrix[] =
	{
		1, 0, 0,
		0, 1, 0,
		1, 2, 1
	};

	iaMatrix3x3d matrix;
	matrix.translate(1, 2);

	IAUX_EXPECT_TRUE(0 == std::memcmp(matrix.getData(), translateMatrix, sizeof(matrix)));
}

IAUX_TEST(Matrix3x3Tests, TranslateNegative)
{
	static const float64 translateNegativeMatrix[] =
	{
		1, 0, 0,
		0, 1, 0,
		-1, -2, 1
	};

	iaMatrix3x3d matrix;
	matrix.translate(-1, -2);

	IAUX_EXPECT_TRUE(0 == std::memcmp(matrix.getData(), translateNegativeMatrix, sizeof(matrix)));
}

IAUX_TEST(Matrix3x3Tests, Scale)
{
	static const float64 scaleMatrix[] =
	{
		10, 0, 0,
		0, 20, 0,
		0, 0,  1
	};


	iaMatrix3x3d matrix;
	matrix.scale(10, 20);

	IAUX_EXPECT_TRUE(0 == std::memcmp(matrix.getData(), scaleMatrix, sizeof(matrix)));
}

IAUX_TEST(Matrix3x3Tests, ScaleNegative)
{
	static const float64 scaleNegativeMatrix[] =
	{
		-10, 0, 0,
		0, -20, 0,
		0, 0,  1
	};

	iaMatrix3x3d matrix;
	matrix.scale(-10, -20);

	IAUX_EXPECT_TRUE(0 == std::memcmp(matrix.getData(), scaleNegativeMatrix, sizeof(matrix)));
}
IAUX_TEST(Matrix3x3Tests, Transpose)
{
	iaMatrix3x3d matrix;

	matrix[1] = 10;
	matrix[2] = 30;

	matrix[3] = 40;
	matrix[6] = 80;

	matrix.transpose();

	static const float64 cmpMatrix[] =
	{
		1, 40, 80,
		10, 1, 0, 
		30, 0, 1
	};

	IAUX_EXPECT_TRUE(0 == std::memcmp(matrix.getData(), cmpMatrix, sizeof(matrix)));
}

// tocalc determinants https://matrix.reshish.com/detCalculation.php
IAUX_TEST(Matrix3x3Tests, DeterminantOfID)
{
	iaMatrix3x3d matrix;

	IAUX_EXPECT_NEAR(matrix.determinant(), 1, 0.00000001);
}

IAUX_TEST(Matrix3x3Tests, DeterminantOfTranslation)
{
	iaMatrix3x3d matrix;
	matrix.translate(1, 2);

	IAUX_EXPECT_NEAR(matrix.determinant(), 1, 0.00000001);
}

IAUX_TEST(Matrix3x3Tests, DeterminantOfScale)
{
	iaMatrix3x3d matrix;
	matrix.scale(2, -3);

	IAUX_EXPECT_NEAR(matrix.determinant(), -30, 0.00000001);
}

IAUX_TEST(Matrix3x3Tests, DeterminantOfRotate)
{
	iaMatrix3x3d matrix;
	matrix.rotate(0.1);

	IAUX_EXPECT_NEAR(matrix.determinant(), 1.0, 0.00000001);
}

IAUX_TEST(Matrix3x3Tests, DeterminantOfMixedMatrix)
{
	iaMatrix3x3d matrix;
	matrix.translate(1, 2);
	matrix.scale(2, -1);
	matrix.rotate(0.1);

	IAUX_EXPECT_NEAR(matrix.determinant(), -30, 0.00000001);
}