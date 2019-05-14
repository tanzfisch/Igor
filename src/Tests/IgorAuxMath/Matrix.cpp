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

TEST(MatrixTests, Rotate)
{
	static const float64 scaleNegativeMatrix[] =
	{
		0.93629336358419923, 0.31299182578546797, -0.15934507930797789, 0,
		-0.28962947762551555, 0.94470248599489426, 0.15379199798896420, 0,
		0.19866933079506122, -0.097843395007255710, 0.97517032720181596, 0,
		0, 0, 0, 1
	};

	iaMatrixd matrix;
	matrix.rotate(0.1, 0.2, 0.3);

	EXPECT_TRUE(0 == std::memcmp(matrix.getData(), scaleNegativeMatrix, sizeof(matrix)));
}

TEST(MatrixTests, DecomposeTranslate)
{
	iaMatrixd matrix;
	matrix.translate(13, 12, 453);

	iaVector3d scale;
	iaQuaterniond orientation;
	iaVector3d translate;
	iaVector3d shear;
	iaVector4d perspective;

	matrix.decompose(scale, orientation, translate, shear, perspective);

	EXPECT_EQ(translate._x, 13);
	EXPECT_EQ(translate._y, 12);
	EXPECT_EQ(translate._z, 453);
}

TEST(MatrixTests, DecomposeTranslateNegative)
{
	iaMatrixd matrix;
	matrix.translate(13, -12, -453);

	iaVector3d scale;
	iaQuaterniond orientation;
	iaVector3d translate;
	iaVector3d shear;
	iaVector4d perspective;

	matrix.decompose(scale, orientation, translate, shear, perspective);

	EXPECT_EQ(translate._x, 13);
	EXPECT_EQ(translate._y, -12);
	EXPECT_EQ(translate._z, -453);
}

TEST(MatrixTests, DecomposeScale)
{
	iaMatrixd matrix;
	matrix.scale(1, 2, 0.003);

	iaVector3d scale;
	iaQuaterniond orientation;
	iaVector3d translate;
	iaVector3d shear;
	iaVector4d perspective;

	matrix.decompose(scale, orientation, translate, shear, perspective);

	EXPECT_EQ(scale._x, 1);
	EXPECT_EQ(scale._y, 2);
	EXPECT_EQ(scale._z, 0.003);
}

TEST(MatrixTests, DecomposeScaleNegative)
{
	iaMatrixd matrix;
	matrix.scale(-1, 2, 0.003);

	iaVector3d scale;
	iaQuaterniond orientation;
	iaVector3d translate;
	iaVector3d shear;
	iaVector4d perspective;

	matrix.decompose(scale, orientation, translate, shear, perspective);

	EXPECT_EQ(scale._x, -1);
	EXPECT_EQ(scale._y, 2);
	EXPECT_EQ(scale._z, 0.003);
}

TEST(MatrixTests, DecomposeShear)
{
	iaMatrixd matrix;
	matrix.shear(5, 0.17,12);

	iaVector3d scale;
	iaQuaterniond orientation;
	iaVector3d translate;
	iaVector3d shear;
	iaVector4d perspective;

	matrix.decompose(scale, orientation, translate, shear, perspective);

	EXPECT_EQ(shear._x, 5);
	EXPECT_EQ(shear._y, 0.17);
	EXPECT_EQ(shear._z, 12);
}

TEST(MatrixTests, DecomposeRotate)
{
	iaMatrixd matrix;
	matrix.rotate(0.1, 0.2, 0.3);

	iaVector3d scale;
	iaQuaterniond orientation;
	iaVector3d translate;
	iaVector3d shear;
	iaVector4d perspective;

	matrix.decompose(scale, orientation, translate, shear, perspective);

	iaVector3d rotate;
	orientation.getEuler(rotate);

	EXPECT_EQ(rotate._x, 0.1);
	EXPECT_EQ(rotate._y, 0.2);
	EXPECT_EQ(rotate._z, 0.3);
}