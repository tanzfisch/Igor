#include "stdafx.h"
#include "CppUnitTest.h"

#include <iaVector3.h>
using namespace IgorAux;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testigor
{

	TEST_CLASS(UnitTest_iaVector3d)
	{
	public:

		TEST_METHOD(Vector2dDefaultCtor)
		{
			iaVector3d vec;

            Assert::IsTrue(vec._x == 0.0 && vec._y == 0.0 && vec._z == 0.0);
		}

		TEST_METHOD(Vector2dParamCtor)
		{
			iaVector3d vec(1.0, -2.0, 3.0);

            Assert::IsTrue(vec._x == 1.0 && vec._y == -2.0 && vec._z == 3.0);
		}

		TEST_METHOD(Vector2dLenghtZero)
		{
			iaVector3d vec;
			float64 lenght = vec.length();

			Assert::AreEqual(lenght, 0.0);
		}

		TEST_METHOD(Vector2dLenght)
		{
			iaVector3d vec(3.0, 0.0, 4.0);
			float64 lenght = vec.length();

			Assert::AreEqual(lenght, 5.0);
		}

		TEST_METHOD(Vector2dLenght2)
		{
			iaVector3d vec(0.0, 3.0, 4.0);
			float64 lenght = vec.length2();

			Assert::AreEqual(lenght, 25.0);
		}

		TEST_METHOD(Vector2dDistance)
		{
			iaVector3d vec1(4.0, 5.0, 3.0);
			iaVector3d vec2(1.0, 1.0, 3.0);
			float64 distance = vec1.distance(vec2);

			Assert::AreEqual(distance, 5.0);
		}

		TEST_METHOD(Vector2dDistance2)
		{
			iaVector3d vec1(4.0, 7.0, 5.0);
			iaVector3d vec2(1.0, 7.0, 1.0);
			float64 distance = vec1.distance2(vec2);

			Assert::AreEqual(distance, 25.0);
		}

		TEST_METHOD(Vector2dAngle)
		{
			iaVector3d vec1(1.0, 0.0, 0.0);
			iaVector3d vec2(0.0, 1.0, 0.0);
			float64 angle = vec1.angle(vec2);

            Assert::AreEqual(angle, static_cast<float64>(M_PI)* 0.5);
		}

		TEST_METHOD(Vector2dSet)
		{
			iaVector3d vec;
			vec.set(1,2,3);

            Assert::IsTrue(vec._x == 1.0 && vec._y == 2.0 && vec._z == 3.0);
		}

		TEST_METHOD(Vector2dGetData)
		{
			float64* data=0;
			iaVector3d vec(2.0, 3.0, 17.5);
			data = vec.getData();

            Assert::IsTrue(data[0] == 2.0 && data[1] == 3.0 && data[2] == 17.5);
		}

		TEST_METHOD(Vector2dNormalize)
		{
			iaVector3d vec(10.2, 0.0, 0.0);
			vec.normalize();

            Assert::IsTrue(fabs(vec._x) > 0.9999999999 && fabs(vec._x) <= 1.0 && 
                vec._y == 0 && vec._z == 0);
		}

	};
}