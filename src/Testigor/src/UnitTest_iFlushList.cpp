#include "stdafx.h"
#include "CppUnitTest.h"

#include <iaFlushVector.h>
using namespace IgorAux;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testigor
{		
	TEST_CLASS(UnitTest_iaFlushList)
	{
	public:

		TEST_METHOD(CreateEmptyList)
		{
			iaFlushVector<int> flushList;

			vector<int> vector = flushList.getList();

			Assert::AreEqual((int)vector.size(), 0);
		}
		
		TEST_METHOD(AddListItem)
		{
			iaFlushVector<int> flushList;

			flushList.add(1);

			vector<int> vector = flushList.getList();

			Assert::AreEqual((int)vector.size(), 0);
		}

		TEST_METHOD(RemoveListItem)
		{
			iaFlushVector<int> flushList;

			flushList.remove(1);

			vector<int> vector = flushList.getList();

			Assert::AreEqual((int)vector.size(), 0);
		}

		TEST_METHOD(RemoveFlushListItem)
		{
			iaFlushVector<int> flushList;

			flushList.remove(1);
			flushList.flush();

			vector<int> vector = flushList.getList();

			Assert::AreEqual((int)vector.size(), 0);
		}

		TEST_METHOD(AddAndFlushListItem)
		{
			iaFlushVector<int> flushList;

			flushList.add(1);
			flushList.flush();

			vector<int> vector = flushList.getList();

			Assert::AreEqual((int)vector.size(), 1);
		}

		TEST_METHOD(DoubleAddAndFlushListItem)
		{
			iaFlushVector<int> flushList;

			flushList.add(1);
			flushList.add(1);
			flushList.flush();

			vector<int> vector = flushList.getList();

			Assert::AreEqual((int)vector.size(), 1);
		}

		TEST_METHOD(AddFlushandRemoveListItem)
		{
			iaFlushVector<int> flushList;

			flushList.add(1);
			flushList.flush();

			flushList.remove(1);
			vector<int> vector = flushList.getList();

			Assert::AreEqual((int)vector.size(), 1);
		}

		TEST_METHOD(AddFlushandRemoveFlushListItem)
		{
			iaFlushVector<int> flushList;

			flushList.add(1);
			flushList.flush();

			flushList.remove(1);
			flushList.flush();

			vector<int> vector = flushList.getList();

			Assert::AreEqual((int)vector.size(), 0);
		}

		TEST_METHOD(AddAndRemoveListItem)
		{
			iaFlushVector<int> flushList;

			flushList.add(1);
			flushList.remove(1);

			vector<int> vector = flushList.getList();

			Assert::AreEqual((int)vector.size(), 0);
		}

		TEST_METHOD(AddAndRemoveFlushListItem)
		{
			iaFlushVector<int> flushList;

			flushList.add(1);
			flushList.remove(1);
			flushList.flush();

			vector<int> vector = flushList.getList();

			Assert::AreEqual((int)vector.size(), 0);
		}

		TEST_METHOD(RemoveAndAddListItem)
		{
			iaFlushVector<int> flushList;

			flushList.remove(1);
			flushList.add(1);

			vector<int> vector = flushList.getList();

			Assert::AreEqual((int)vector.size(), 0);
		}

		TEST_METHOD(RemoveAndAddFlushListItem)
		{
			iaFlushVector<int> flushList;

			flushList.remove(1);
			flushList.add(1);
			flushList.flush();

			vector<int> vector = flushList.getList();

			Assert::AreEqual((int)vector.size(), 1);
		}

	};
}