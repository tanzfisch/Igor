#include "stdafx.h"
#include "CppUnitTest.h"

#include <iaVector2.h>
using namespace IgorAux;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testigor
{

	TEST_CLASS(UnitTest_iaVector2f)
	{
	public:

		TEST_METHOD(Vector2fDefaultCtor)
		{
			iaVector2f vec;

			Assert::IsTrue(vec.x == 0.0f && 
				vec.y == 0.0f);
		}

		TEST_METHOD(Vector2fParamCtor)
		{
			iaVector2f vec(1.0f,2.0f);

			Assert::IsTrue(vec.x == 1.0f && 
				vec.y == 2.0f);
		}

		TEST_METHOD(Vector2fLenghtZero)
		{
			iaVector2f vec;
			float32 lenght = vec.length();

			Assert::AreEqual(lenght, 0.0f);
		}

		TEST_METHOD(Vector2fLenght)
		{
			iaVector2f vec(3.0f,4.0f);
			float32 lenght = vec.length();

			Assert::AreEqual(lenght, 5.0f);
		}

		TEST_METHOD(Vector2fLenght2)
		{
			iaVector2f vec(3.0f,4.0f);
			float32 lenght = vec.length2();

			Assert::AreEqual(lenght, 25.0f);
		}

		TEST_METHOD(Vector2fDistance)
		{
			iaVector2f vec1(4.0f,5.0f);
			iaVector2f vec2(1.0f,1.0f);
			float32 distance = vec1.distance(vec2);

			Assert::AreEqual(distance, 5.0f);
		}

		TEST_METHOD(Vector2fDistance2)
		{
			iaVector2f vec1(4.0f,5.0f);
			iaVector2f vec2(1.0f,1.0f);
			float32 distance = vec1.distance2(vec2);

			Assert::AreEqual(distance, 25.0f);
		}

		TEST_METHOD(Vector2fAngleX)
		{
			iaVector2f vec(0.0f,1.0f);
			float32 angle = vec.angleX();

            Assert::AreEqual(angle, static_cast<float32>(M_PI)* 0.5f);
		}

		TEST_METHOD(Vector2fAngleY)
		{
			iaVector2f vec(-1.0f,0.0f);
			float32 angle = vec.angleY();

            Assert::AreEqual(angle, static_cast<float32>(M_PI)* 0.5f);
		}

		TEST_METHOD(Vector2fAngle)
		{
			iaVector2f vec1(1.0f,0.0f);
			iaVector2f vec2(0.0f,1.0f);
			float32 angle = vec1.angle(vec2);

            Assert::AreEqual(angle, static_cast<float32>(M_PI) * 0.5f);
		}

		TEST_METHOD(Vector2fSet)
		{
			iaVector2f vec;
			vec.set(1,2);

			Assert::IsTrue(vec.x == 1.0f && 
				vec.y == 2.0f);
		}

		TEST_METHOD(Vector2fGetData)
		{
			float32* data=0;
			iaVector2f vec(2,3);
			data = vec.getData();

			Assert::IsTrue(data[0] == 2.0f && 
				data[1] == 3.0f);
		}

		TEST_METHOD(Vector2fRotateXY)
		{
			iaVector2f vec(1.0f,0.0f);
			vec.rotateXY(0.5f * static_cast<float32>(M_PI));

			Assert::IsTrue(fabs(vec.x) < 0.00001f && 
				fabs(vec.y) > 0.99999f);
		}

		TEST_METHOD(Vector2fNormalize)
		{
			iaVector2f vec(10.2f,0.0f);
			vec.normalize();

			Assert::IsTrue(fabs(vec.x) > 0.99999f && 
				fabs(vec.x) <= 1.0f &&
				vec.y == 0);
		}

	};
}