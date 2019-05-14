#include "pch.h"
#include <iaMatrix.h>
using namespace IgorAux;

static const float64 identityMatrix[] =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1 
};

TEST(MatrixTests, Initial) 
{
	iaMatrixd matrix;

	EXPECT_TRUE(0 == std::memcmp(matrix.getData(), identityMatrix, sizeof(matrix)));
}

TEST(MatrixTests, Identity) 
{
	iaMatrixd matrix;
	matrix.identity();

	EXPECT_TRUE(0 == std::memcmp(matrix.getData(), identityMatrix, sizeof(matrix)));
}

TEST(MatrixTests, Translate)
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

	EXPECT_TRUE(0 == std::memcmp(matrix.getData(), translateMatrix, sizeof(matrix)));
}

TEST(MatrixTests, TranslateNegative)
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

	EXPECT_TRUE(0 == std::memcmp(matrix.getData(), translateNegativeMatrix, sizeof(matrix)));
}

TEST(MatrixTests, Scale)
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

	EXPECT_TRUE(0 == std::memcmp(matrix.getData(), scaleMatrix, sizeof(matrix)));
}

TEST(MatrixTests, ScaleNegative)
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

	EXPECT_TRUE(0 == std::memcmp(matrix.getData(), scaleNegativeMatrix, sizeof(matrix)));
}
