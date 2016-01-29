#include "stdafx.h"
#include "CppUnitTest.h"

#include <iaVector2.h>
using namespace IgorAux;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testigor
{

	TEST_CLASS(UnitTest_iVector2d)
	{
	public:

		TEST_METHOD(Vector2dDefaultCtor)
		{
			iaVector2d vec;

			Assert::IsTrue(vec.x == 0.0 && 
				vec.y == 0.0);
		}

		TEST_METHOD(Vector2dParamCtor)
		{
			iaVector2d vec(1.0,2.0);

			Assert::IsTrue(vec.x == 1.0 && 
				vec.y == 2.0);
		}

		TEST_METHOD(Vector2dLenghtZero)
		{
			iaVector2d vec;
			float64 lenght = vec.length();

			Assert::AreEqual(lenght, 0.0);
		}

		TEST_METHOD(Vector2dLenght)
		{
			iaVector2d vec(3.0,4.0);
			float64 lenght = vec.length();

			Assert::AreEqual(lenght, 5.0);
		}

		TEST_METHOD(Vector2dLenght2)
		{
			iaVector2d vec(3.0,4.0);
			float64 lenght = vec.length2();

			Assert::AreEqual(lenght, 25.0);
		}

		TEST_METHOD(Vector2dDistance)
		{
			iaVector2d vec1(4.0,5.0);
			iaVector2d vec2(1.0,1.0);
			float64 distance = vec1.distance(vec2);

			Assert::AreEqual(distance, 5.0);
		}

		TEST_METHOD(Vector2dDistance2)
		{
			iaVector2d vec1(4.0,5.0);
			iaVector2d vec2(1.0,1.0);
			float64 distance = vec1.distance2(vec2);

			Assert::AreEqual(distance, 25.0);
		}

		TEST_METHOD(Vector2dAngleX)
		{
			iaVector2d vec(0.0,1.0);
			float64 angle = vec.angleX();

            Assert::AreEqual(angle, static_cast<float64>(M_PI) * 0.5);
		}

		TEST_METHOD(Vector2dAngleY)
		{
			iaVector2d vec(-1.0,0.0);
			float64 angle = vec.angleY();

            Assert::AreEqual(angle, static_cast<float64>(M_PI)* 0.5);
		}

		TEST_METHOD(Vector2dAngle)
		{
			iaVector2d vec1(1.0,0.0);
			iaVector2d vec2(0.0,1.0);
			float64 angle = vec1.angle(vec2);

            Assert::AreEqual(angle, static_cast<float64>(M_PI)* 0.5);
		}

		TEST_METHOD(Vector2dSet)
		{
			iaVector2d vec;
			vec.set(1,2);

			Assert::IsTrue(vec.x == 1.0 && 
				vec.y == 2.0);
		}

		TEST_METHOD(Vector2dGetData)
		{
			float64* data=0;
			iaVector2d vec(2,3);
			data = vec.getData();

			Assert::IsTrue(data[0] == 2.0 && 
				data[1] == 3.0);
		}

		TEST_METHOD(Vector2dRotateXY)
		{
			iaVector2d vec(1.0,0.0);
			vec.rotateXY(0.5 * static_cast<float64>(M_PI));

			Assert::IsTrue(fabs(vec.x) < 0.0000000001 && 
                fabs(vec.y) > 0.9999999999);
		}

		TEST_METHOD(Vector2dNormalize)
		{
			iaVector2d vec(10.2, 0.0);
			vec.normalize();

            Assert::IsTrue(fabs(vec.x) > 0.9999999999 &&
				fabs(vec.x) <= 1.0 &&
				vec.y == 0);
		}

	};
}