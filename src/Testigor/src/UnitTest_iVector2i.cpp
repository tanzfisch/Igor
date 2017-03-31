#include "stdafx.h"
#include "CppUnitTest.h"

#include <iaVector2.h>
using namespace IgorAux;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testigor
{

	TEST_CLASS(UnitTest_iaVector2i)
	{
	public:

		TEST_METHOD(Vector2iDefaultCtor)
		{
			iaVector2i vec;

			Assert::IsTrue(vec._x == 0 && vec._y == 0);
		}

		TEST_METHOD(Vector2iParamCtor)
		{
			iaVector2i vec(1, 2);

			Assert::IsTrue(vec._x == 1 && vec._y == 2);
		}

		TEST_METHOD(Vector2iLenghtZero)
		{
			iaVector2i vec;
			int32 lenght = vec.length();

			Assert::AreEqual(lenght, 0);
		}

		TEST_METHOD(Vector2iLenght)
		{
			iaVector2i vec(3, 4);
			int32 lenght = vec.length();

			Assert::AreEqual(lenght, 5);
		}

		TEST_METHOD(Vector2iLenght2)
		{
			iaVector2i vec(3, 4);
			int32 lenght = vec.length2();

			Assert::AreEqual(lenght, 25);
		}

		TEST_METHOD(Vector2iDistance)
		{
			iaVector2i vec1(4, 5);
			iaVector2i vec2(1, 1);
			int32 distance = vec1.distance(vec2);

			Assert::AreEqual(distance, 5);
		}

		TEST_METHOD(Vector2iDistance2)
		{
			iaVector2i vec1(4, 5);
			iaVector2i vec2(1, 1);
			int32 distance = vec1.distance2(vec2);

			Assert::AreEqual(distance, 25);
		}

		TEST_METHOD(Vector2iSet)
		{
			iaVector2i vec;
			vec.set(1, 2);

			Assert::IsTrue(vec._x == 1 && vec._y == 2);
		}

		TEST_METHOD(Vector2iGetData)
		{
			int32* data=0;
			iaVector2i vec(2, 3);
			data = vec.getData();

			Assert::IsTrue(data[0] == 2 && data[1] == 3);
		}

		TEST_METHOD(Vector2iNormalize)
		{
			iaVector2i vec(10, 0);
			vec.normalize();

            Assert::AreEqual(vec._x, 1);
            Assert::AreEqual(vec._y, 0);
		}

	};
}