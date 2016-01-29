#include "stdafx.h"
#include "CppUnitTest.h"

#include <iaFlushList.h>
using namespace IgorAux;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testigor
{		
	TEST_CLASS(UnitTest_iaFlushList)
	{
	public:

		TEST_METHOD(CreateEmptyList)
		{
			iaFlushList<int> flushList;

			list<int> list = flushList.getList();

			Assert::AreEqual((int)list.size(), 0);
		}
		
		TEST_METHOD(AddListItem)
		{
			iaFlushList<int> flushList;

			flushList.add(1);

			list<int> list = flushList.getList();

			Assert::AreEqual((int)list.size(), 0);
		}

		TEST_METHOD(RemoveListItem)
		{
			iaFlushList<int> flushList;

			flushList.remove(1);

			list<int> list = flushList.getList();

			Assert::AreEqual((int)list.size(), 0);
		}

		TEST_METHOD(RemoveFlushListItem)
		{
			iaFlushList<int> flushList;

			flushList.remove(1);
			flushList.flush();

			list<int> list = flushList.getList();

			Assert::AreEqual((int)list.size(), 0);
		}

		TEST_METHOD(AddAndFlushListItem)
		{
			iaFlushList<int> flushList;

			flushList.add(1);
			flushList.flush();

			list<int> list = flushList.getList();

			Assert::AreEqual((int)list.size(), 1);
		}

		TEST_METHOD(DoubleAddAndFlushListItem)
		{
			iaFlushList<int> flushList;

			flushList.add(1);
			flushList.add(1);
			flushList.flush();

			list<int> list = flushList.getList();

			Assert::AreEqual((int)list.size(), 1);
		}

		TEST_METHOD(AddFlushandRemoveListItem)
		{
			iaFlushList<int> flushList;

			flushList.add(1);
			flushList.flush();

			flushList.remove(1);
			list<int> list = flushList.getList();

			Assert::AreEqual((int)list.size(), 1);
		}

		TEST_METHOD(AddFlushandRemoveFlushListItem)
		{
			iaFlushList<int> flushList;

			flushList.add(1);
			flushList.flush();

			flushList.remove(1);
			flushList.flush();

			list<int> list = flushList.getList();

			Assert::AreEqual((int)list.size(), 0);
		}

		TEST_METHOD(AddAndRemoveListItem)
		{
			iaFlushList<int> flushList;

			flushList.add(1);
			flushList.remove(1);

			list<int> list = flushList.getList();

			Assert::AreEqual((int)list.size(), 0);
		}

		TEST_METHOD(AddAndRemoveFlushListItem)
		{
			iaFlushList<int> flushList;

			flushList.add(1);
			flushList.remove(1);
			flushList.flush();

			list<int> list = flushList.getList();

			Assert::AreEqual((int)list.size(), 0);
		}

		TEST_METHOD(RemoveAndAddListItem)
		{
			iaFlushList<int> flushList;

			flushList.remove(1);
			flushList.add(1);

			list<int> list = flushList.getList();

			Assert::AreEqual((int)list.size(), 0);
		}

		TEST_METHOD(RemoveAndAddFlushListItem)
		{
			iaFlushList<int> flushList;

			flushList.remove(1);
			flushList.add(1);
			flushList.flush();

			list<int> list = flushList.getList();

			Assert::AreEqual((int)list.size(), 1);
		}

	};
}