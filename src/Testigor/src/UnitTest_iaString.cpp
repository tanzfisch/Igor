#include "stdafx.h"
#include "CppUnitTest.h"

#include <iaString.h>
using namespace IgorAux;

#include <sstream>
using namespace std;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testigor
{
	TEST_CLASS(UnitTest_iaStringTest)
	{
	public:

        TEST_METHOD(UnitTest_iaStringTest_1)
        {
            iaString text;
            Assert::IsTrue(text.getSize() == 0);
            Assert::IsTrue(text.getData() == nullptr);
            Assert::IsTrue(text.isEmpty() == true);
        }

		TEST_METHOD(UnitTest_iaStringTest_2)
		{
		    iaString text("12345");
            Assert::IsTrue(text.getSize() == 5);
            Assert::IsTrue(text.getData()[5] == 0);
            Assert::IsTrue(text.isEmpty() == false);
		}

        TEST_METHOD(UnitTest_iaStringTest_3)
        {
            iaString text(L"12345");
            Assert::IsTrue(text.getSize() == 5);
            Assert::IsTrue(text.getData()[5] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_4)
        {
            iaString text("12345", 3);
            Assert::IsTrue(text.getSize() == 3);
            Assert::IsTrue(text.getData()[3] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_5)
        {
            iaString text(L"12345", 3);
            Assert::IsTrue(text.getSize() == 3);
            Assert::IsTrue(text.getData()[3] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_6)
        {
            iaString text("12345", iaString::INVALID_POSITION);
            Assert::IsTrue(text.getSize() == 5);
            Assert::IsTrue(text.getData()[5] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_7)
        {
            iaString text(L"12345", iaString::INVALID_POSITION);
            Assert::IsTrue(text.getSize() == 5);
            Assert::IsTrue(text.getData()[5] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_8)
        {
            iaString text(static_cast<const char*>(nullptr));
            Assert::IsTrue(text.getSize() == 0);
            Assert::IsTrue(text.getData() == nullptr);
            Assert::IsTrue(text.isEmpty() == true);
        }

        TEST_METHOD(UnitTest_iaStringTest_9)
        {
            iaString text(static_cast<const wchar_t*>(nullptr));
            Assert::IsTrue(text.getSize() == 0);
            Assert::IsTrue(text.getData() == nullptr);
            Assert::IsTrue(text.isEmpty() == true);
        }

        TEST_METHOD(UnitTest_iaStringTest_10)
        {
            iaString text(static_cast<const char*>(nullptr), iaString::INVALID_POSITION);
            Assert::IsTrue(text.getSize() == 0);
            Assert::IsTrue(text.getData() == nullptr);
            Assert::IsTrue(text.isEmpty() == true);
        }

        TEST_METHOD(UnitTest_iaStringTest_11)
        {
            iaString text(static_cast<const wchar_t*>(nullptr), iaString::INVALID_POSITION);
            Assert::IsTrue(text.getSize() == 0);
            Assert::IsTrue(text.getData() == nullptr);
            Assert::IsTrue(text.isEmpty() == true);
        }

        TEST_METHOD(UnitTest_iaStringTest_12)
        {
            iaString foo;
            iaString bar(foo);
            Assert::IsTrue(foo.getSize() == 0);
            Assert::IsTrue(foo.getData() == nullptr);
            Assert::IsTrue(foo.isEmpty() == true);
            Assert::IsTrue(bar.getSize() == 0);
            Assert::IsTrue(bar.getData() == nullptr);
            Assert::IsTrue(bar.isEmpty() == true);
        }
        
        TEST_METHOD(UnitTest_iaStringTest_13)
        {
            iaString foo("12345");
            iaString bar(foo);
            Assert::IsTrue(foo.getSize() == 5);
            Assert::IsTrue(foo.getData()[5] == 0);
            Assert::IsTrue(foo.isEmpty() == false);
            Assert::IsTrue(bar.getSize() == 5);
            Assert::IsTrue(bar.getData()[5] == 0);
            Assert::IsTrue(bar.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_14)
        {
            iaString foo("12345", 3);
            iaString bar(foo);
            Assert::IsTrue(foo.getSize() == 3);
            Assert::IsTrue(foo.getData()[3] == 0);
            Assert::IsTrue(foo.isEmpty() == false);
            Assert::IsTrue(bar.getSize() == 3);
            Assert::IsTrue(bar.getData()[3] == 0);
            Assert::IsTrue(bar.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_15)
        {
            iaString foo;
            iaString bar("12345");

            foo = bar;

            Assert::IsTrue(foo.getSize() == 5);
            Assert::IsTrue(foo.getData()[5] == 0);
            Assert::IsTrue(foo.isEmpty() == false);
            Assert::IsTrue(bar.getSize() == 5);
            Assert::IsTrue(bar.getData()[5] == 0);
            Assert::IsTrue(bar.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_16)
        {
            iaString foo;
            iaString bar("12345");

            foo = foo + bar;

            Assert::IsTrue(foo.getSize() == 5);
            Assert::IsTrue(foo.getData()[5] == 0);
            Assert::IsTrue(foo.isEmpty() == false);
            Assert::IsTrue(bar.getSize() == 5);
            Assert::IsTrue(bar.getData()[5] == 0);
            Assert::IsTrue(bar.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_17)
        {
            iaString foo;
            iaString bar("12345");

            foo += bar;

            Assert::IsTrue(foo.getSize() == 5);
            Assert::IsTrue(foo.getData()[5] == 0);
            Assert::IsTrue(foo.isEmpty() == false);
            Assert::IsTrue(bar.getSize() == 5);
            Assert::IsTrue(bar.getData()[5] == 0);
            Assert::IsTrue(bar.isEmpty() == false);
        }    

        TEST_METHOD(UnitTest_iaStringTest_18)
        {
            iaString foo("abc");
            iaString bar("12345");

            foo = bar;

            Assert::IsTrue(foo.getSize() == 5);
            Assert::IsTrue(foo.getData()[5] == 0);
            Assert::IsTrue(foo.isEmpty() == false);
            Assert::IsTrue(bar.getSize() == 5);
            Assert::IsTrue(bar.getData()[5] == 0);
            Assert::IsTrue(bar.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_19)
        {
            iaString foo("abc");
            iaString bar("12345");

            foo = foo + bar;

            Assert::IsTrue(foo.getSize() == 8);
            Assert::IsTrue(foo.getData()[8] == 0);
            Assert::IsTrue(foo.isEmpty() == false);
            Assert::IsTrue(bar.getSize() == 5);
            Assert::IsTrue(bar.getData()[5] == 0);
            Assert::IsTrue(bar.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_20)
        {
            iaString foo("abc");
            iaString bar("12345");

            foo += bar;

            Assert::IsTrue(foo.getSize() == 8);
            Assert::IsTrue(foo.getData()[8] == 0);
            Assert::IsTrue(foo.isEmpty() == false);
            Assert::IsTrue(bar.getSize() == 5);
            Assert::IsTrue(bar.getData()[5] == 0);
            Assert::IsTrue(bar.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_21)
        {
            iaString text;
            text = 'a';

            Assert::IsTrue(text.getSize() == 1);
            Assert::IsTrue(text.getData()[1] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_22)
        {
            iaString text;
            text = L'a';

            Assert::IsTrue(text.getSize() == 1);
            Assert::IsTrue(text.getData()[1] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }
        
        TEST_METHOD(UnitTest_iaStringTest_23)
        {
            iaString text;
            text = text + 'a';

            Assert::IsTrue(text.getSize() == 1);
            Assert::IsTrue(text.getData()[1] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_24)
        {
            iaString text;
            text = text + L'a';

            Assert::IsTrue(text.getSize() == 1);
            Assert::IsTrue(text.getData()[1] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_25)
        {
            iaString text;
            text += 'a';

            Assert::IsTrue(text.getSize() == 1);
            Assert::IsTrue(text.getData()[1] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_26)
        {
            iaString text;
            text += L'a';

            Assert::IsTrue(text.getSize() == 1);
            Assert::IsTrue(text.getData()[1] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_27)
        {
            iaString text("123");
            text = text + 'a';

            Assert::IsTrue(text.getSize() == 4);
            Assert::IsTrue(text.getData()[4] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_28)
        {
            iaString text("123");
            text = text + L'a';

            Assert::IsTrue(text.getSize() == 4);
            Assert::IsTrue(text.getData()[4] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_29)
        {
            iaString text("123");
            text += 'a';

            Assert::IsTrue(text.getSize() == 4);
            Assert::IsTrue(text.getData()[4] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_30)
        {
            iaString text("123");
            text += L'a';

            Assert::IsTrue(text.getSize() == 4);
            Assert::IsTrue(text.getData()[4] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_31)
        {
            iaString text;
            text = "123";

            Assert::IsTrue(text.getSize() == 3);
            Assert::IsTrue(text.getData()[3] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_32)
        {
            iaString text;
            text = L"123";

            Assert::IsTrue(text.getSize() == 3);
            Assert::IsTrue(text.getData()[3] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_33)
        {
            iaString text;
            text = text + "123";

            Assert::IsTrue(text.getSize() == 3);
            Assert::IsTrue(text.getData()[3] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_34)
        {
            iaString text;
            text = text + L"123";

            Assert::IsTrue(text.getSize() == 3);
            Assert::IsTrue(text.getData()[3] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_35)
        {
            iaString text;
            text += "123";

            Assert::IsTrue(text.getSize() == 3);
            Assert::IsTrue(text.getData()[3] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_36)
        {
            iaString text;
            text += L"123";

            Assert::IsTrue(text.getSize() == 3);
            Assert::IsTrue(text.getData()[3] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_37)
        {
            iaString text("abc");
            text = text + "123";

            Assert::IsTrue(text.getSize() == 6);
            Assert::IsTrue(text.getData()[6] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_38)
        {
            iaString text("abc");
            text = text + L"123";

            Assert::IsTrue(text.getSize() == 6);
            Assert::IsTrue(text.getData()[6] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_39)
        {
            iaString text("abc");
            text += "123";

            Assert::IsTrue(text.getSize() == 6);
            Assert::IsTrue(text.getData()[6] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_40)
        {
            iaString text("abc");
            text += L"123";

            Assert::IsTrue(text.getSize() == 6);
            Assert::IsTrue(text.getData()[6] == 0);
            Assert::IsTrue(text.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_41)
        {
            iaString text("abc");
            text.clear();

            Assert::IsTrue(text.getSize() == 0);
            Assert::IsTrue(text.getData() == nullptr);
            Assert::IsTrue(text.isEmpty() == true);
        }

        TEST_METHOD(UnitTest_iaStringTest_42)
        {
            iaString foo("abc");
            iaString bar("abc");

            Assert::IsTrue(foo == bar);
        }

        TEST_METHOD(UnitTest_iaStringTest_43)
        {
            iaString foo("abc");
            iaString bar("123");

            Assert::IsTrue(foo != bar);
        }
        
        TEST_METHOD(UnitTest_iaStringTest_44)
        {
            iaString foo("abc");
            iaString bar("abcd");

            Assert::IsTrue(foo != bar);
        }
        
        TEST_METHOD(UnitTest_iaStringTest_45)
        {
            iaString foo;
            iaString bar("abcd");

            Assert::IsTrue(foo != bar);
        }

        TEST_METHOD(UnitTest_iaStringTest_46)
        {
            iaString foo("abc");
            iaString bar;

            Assert::IsTrue(foo != bar);
        }

        TEST_METHOD(UnitTest_iaStringTest_47)
        {
            iaString foo("abc");
            iaString bar("123");

            Assert::IsFalse(foo == bar);
        }

        TEST_METHOD(UnitTest_iaStringTest_48)
        {
            iaString foo("abc");
            iaString bar("abcd");

            Assert::IsFalse(foo == bar);
        }

        TEST_METHOD(UnitTest_iaStringTest_49)
        {
            iaString foo;
            iaString bar("abcd");

            Assert::IsFalse(foo == bar);
        }    
        
        TEST_METHOD(UnitTest_iaStringTest_50)
        {
            iaString foo("abc");
            iaString bar;

            Assert::IsFalse(foo == bar);
        }

        TEST_METHOD(UnitTest_iaStringTest_51)
        {
            iaString foo("abc");

            Assert::IsTrue(foo[0] == L'a');
            Assert::IsTrue(foo[1] == L'b');
            Assert::IsTrue(foo[2] == L'c');
            Assert::IsTrue(foo.getSize() == 3);
            Assert::IsTrue(foo.getData()[3] == 0);
        }

        TEST_METHOD(UnitTest_iaStringTest_52)
        {
            iaString foo("abc");

            foo[2] = L'Y';

            Assert::IsTrue(foo[0] == L'a');
            Assert::IsTrue(foo[1] == L'b');
            Assert::IsTrue(foo[2] == L'Y');
            Assert::IsTrue(foo.getData()[3] == 0);
        }

        TEST_METHOD(UnitTest_iaStringTest_53)
        {
            iaString foo("abcdefghijkl");
            iaString bar = foo.getSubString(0, 3);

            Assert::IsTrue(bar[0] == L'a');
            Assert::IsTrue(bar[1] == L'b');
            Assert::IsTrue(bar[2] == L'c');
            Assert::IsTrue(bar.getData()[3] == 0);
            Assert::IsTrue(bar.getSize() == 3);
            Assert::IsTrue(bar.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_54)
        {
            iaString foo("abcdefghijkl");
            iaString bar = foo.getSubString(0);
            
            Assert::IsTrue(bar.getSize() == 12);
            Assert::IsTrue(bar.getData()[12] == 0);
            Assert::IsTrue(bar.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_55)
        {
            iaString foo("abcdefghijkl");
            iaString bar = foo.getSubString(5);

            Assert::IsTrue(bar.getSize() == 7);
            Assert::IsTrue(bar.getData()[7] == 0);
            Assert::IsTrue(bar == L"fghijkl");
            Assert::IsTrue(bar.isEmpty() == false);
        }    

        TEST_METHOD(UnitTest_iaStringTest_56)
        {
            iaString foo("abcdefghijkl");
            
            Assert::IsTrue(foo == L"abcdefghijkl");
            Assert::IsTrue(foo == "abcdefghijkl");
        }

        TEST_METHOD(UnitTest_iaStringTest_57)
        {
            iaString foo("abcdefghijkl");
            iaString bar = foo.getSubString(5, 3);

            Assert::IsTrue(bar.getSize() == 3);
            Assert::IsTrue(bar.getData()[3] == 0);
            Assert::IsTrue(bar == L"fgh");
            Assert::IsTrue(bar.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_58)
        {
            iaString foo("abcdefghijkl");
            iaString bar = foo.getSubString(5, 100);

            Assert::IsTrue(bar.getSize() == 7);
            Assert::IsTrue(bar.getData()[7] == 0);
            Assert::IsTrue(bar == L"fghijkl");
            Assert::IsTrue(bar.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_59)
        {
            iaString foo("abcdefghijkl");
            iaString bar = foo.getSubString(0, iaString::INVALID_POSITION);

            Assert::IsTrue(bar.getSize() == 12);
            Assert::IsTrue(bar.getData()[12] == 0);
            Assert::IsTrue(bar.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_60)
        {
            iaString foo("abc defg  hij kl");
            vector<iaString> tokens;
            foo.split(" ", tokens);

            Assert::IsTrue(tokens.size() == 4);
            Assert::IsTrue(tokens[0] == "abc");
            Assert::IsTrue(tokens[1] == "defg");
            Assert::IsTrue(tokens[2] == "hij");
            Assert::IsTrue(tokens[3] == "kl");

            Assert::IsTrue(foo.getSize() == 16);
            Assert::IsTrue(foo.getData()[16] == 0);
            Assert::IsTrue(foo.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_61)
        {
            iaString foo("abc de; fg  hij kl");
            vector<iaString> tokens;
            foo.split("; ", tokens);

            Assert::IsTrue(tokens.size() == 5);
            Assert::IsTrue(tokens[0] == "abc");
            Assert::IsTrue(tokens[1] == "de");
            Assert::IsTrue(tokens[2] == "fg");
            Assert::IsTrue(tokens[3] == "hij");
            Assert::IsTrue(tokens[4] == "kl");

            Assert::IsTrue(foo.getSize() == 18);
            Assert::IsTrue(foo.getData()[18] == 0);
            Assert::IsTrue(foo.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_62)
        {
            iaString foo("abc de; fg  hij kl");
            vector<iaString> tokens;
            foo.split(" ;", tokens);

            Assert::IsTrue(tokens.size() == 5);
            Assert::IsTrue(tokens[0] == "abc");
            Assert::IsTrue(tokens[1] == "de");
            Assert::IsTrue(tokens[2] == "fg");
            Assert::IsTrue(tokens[3] == "hij");
            Assert::IsTrue(tokens[4] == "kl");

            Assert::IsTrue(foo.getSize() == 18);
            Assert::IsTrue(foo.getData()[18] == 0);
            Assert::IsTrue(foo.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_63)
        {
            iaString foo("abc de; fg  hij kl");
            vector<iaString> tokens;
            foo.split(" ;x", tokens);

            Assert::IsTrue(tokens.size() == 5);
            Assert::IsTrue(tokens[0] == "abc");
            Assert::IsTrue(tokens[1] == "de");
            Assert::IsTrue(tokens[2] == "fg");
            Assert::IsTrue(tokens[3] == "hij");
            Assert::IsTrue(tokens[4] == "kl");

            Assert::IsTrue(foo.getSize() == 18);
            Assert::IsTrue(foo.getData()[18] == 0);
            Assert::IsTrue(foo.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_64)
        {
            iaString foo("abc");
            vector<iaString> tokens;
            foo.split(" ;x", tokens);

            Assert::IsTrue(tokens.size() == 1);
            Assert::IsTrue(tokens[0] == "abc");

            Assert::IsTrue(foo.getSize() == 3);
            Assert::IsTrue(foo.getData()[3] == 0);
            Assert::IsTrue(foo.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_65)
        {
            iaString foo("abc");
            uint64 result = foo.findFirstOf(L'b');

            Assert::IsTrue(result == 1);
            Assert::IsTrue(foo.getSize() == 3);
            Assert::IsTrue(foo.getData()[3] == 0);
            Assert::IsTrue(foo.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_66)
        {
            iaString foo("abc");
            uint64 result = foo.findFirstOf(L"b");

            Assert::IsTrue(result == 1);
            Assert::IsTrue(foo.getSize() == 3);
            Assert::IsTrue(foo.getData()[3] == 0);
            Assert::IsTrue(foo.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_67)
        {
            iaString foo("abc");
            uint64 result = foo.findFirstOf(L'd');

            Assert::IsTrue(result == iaString::INVALID_POSITION);
            Assert::IsTrue(foo.getSize() == 3);
            Assert::IsTrue(foo.getData()[3] == 0);
            Assert::IsTrue(foo.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_68)
        {
            iaString foo("abc");
            uint64 result = foo.findFirstOf(L"d");

            Assert::IsTrue(result == iaString::INVALID_POSITION);
            Assert::IsTrue(foo.getSize() == 3);
            Assert::IsTrue(foo.getData()[3] == 0);
            Assert::IsTrue(foo.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_69)
        {
            iaString foo("abc");
            uint64 result = foo.findFirstOf(L"def");

            Assert::IsTrue(result == iaString::INVALID_POSITION);
            Assert::IsTrue(foo.getSize() == 3);
            Assert::IsTrue(foo.getData()[3] == 0);
            Assert::IsTrue(foo.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_70)
        {
            iaString foo("abcdefghijk");
            uint64 result = foo.findFirstOf(L"d");

            Assert::IsTrue(result == 3);
        }

        TEST_METHOD(UnitTest_iaStringTest_71)
        {
            iaString foo("abc*fghi*jk");
            uint64 result = foo.findFirstOf(L"*d");

            Assert::IsTrue(result == 3);
        }

        TEST_METHOD(UnitTest_iaStringTest_72)
        {
            iaString foo("abc*fghi*jk");
            uint64 result = foo.findFirstOf(L"*d", 4);

            Assert::IsTrue(result == 8);
        }

        TEST_METHOD(UnitTest_iaStringTest_73)
        {
            iaString foo("abc*fghi*jk");
            uint64 result = foo.findFirstOf(L"*d", 100);

            Assert::IsTrue(result == iaString::INVALID_POSITION);
        }

        TEST_METHOD(UnitTest_iaStringTest_74)
        {
            iaString foo("abc*fghi*jk");
            uint64 result = foo.findLastOf(L"*d");

            Assert::IsTrue(result == 8);
        }
        
        TEST_METHOD(UnitTest_iaStringTest_75)
        {
            iaString foo("abc*fghi*jk");
            uint64 result = foo.findLastOf(L'*');

            Assert::IsTrue(result == 8);
        }

        TEST_METHOD(UnitTest_iaStringTest_76)
        {
            iaString foo("abc");
            char text[10];
            uint64 result = foo.getData(text, 10);

            Assert::IsTrue(text[0] == 'a');
            Assert::IsTrue(text[1] == 'b');
            Assert::IsTrue(text[2] == 'c');
            Assert::IsTrue(text[3] == 0);
            Assert::IsTrue(result == 4);
        }

        TEST_METHOD(UnitTest_iaStringTest_77)
        {
            iaString foo("abc");
            char text[10];
            uint64 result = foo.getData(text, 3);

            Assert::IsTrue(text[0] == 'a');
            Assert::IsTrue(text[1] == 'b');
            Assert::IsTrue(text[2] == 0);
            Assert::IsTrue(result == 3);
        }

        TEST_METHOD(UnitTest_iaStringTest_79)
        {
            wstringstream stream;
            iaString foo("abc");

            stream << foo;

            Assert::IsTrue(stream.str() == L"abc");
        }

        TEST_METHOD(UnitTest_iaStringTest_80)
        {
            iaString foo("abc");
            uint64 result = foo.findFirstNotOf(L"ac");

            Assert::IsTrue(result == 1);
            Assert::IsTrue(foo.getSize() == 3);
            Assert::IsTrue(foo.getData()[3] == 0);
            Assert::IsTrue(foo.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_81)
        {
            iaString foo("abc");
            uint64 result = foo.findFirstNotOf(L"abc");

            Assert::IsTrue(result == iaString::INVALID_POSITION);
            Assert::IsTrue(foo.getSize() == 3);
            Assert::IsTrue(foo.getData()[3] == 0);
            Assert::IsTrue(foo.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_82)
        {
            iaString foo;
            uint64 result = foo.findFirstNotOf(L"ac");

            Assert::IsTrue(result == iaString::INVALID_POSITION);
            Assert::IsTrue(foo.getSize() == 0);
            Assert::IsTrue(foo.getData() == nullptr);
            Assert::IsTrue(foo.isEmpty() == true);
        }


        TEST_METHOD(UnitTest_iaStringTest_83)
        {
            iaString foo("abc");
            uint64 result = foo.findFirstNotOf(L'a');

            Assert::IsTrue(result == 1);
            Assert::IsTrue(foo.getSize() == 3);
            Assert::IsTrue(foo.getData()[3] == 0);
            Assert::IsTrue(foo.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_84)
        {
            iaString foo;
            uint64 result = foo.findFirstNotOf(L'a');

            Assert::IsTrue(result == iaString::INVALID_POSITION);
            Assert::IsTrue(foo.getSize() == 0);
            Assert::IsTrue(foo.getData() == nullptr);
            Assert::IsTrue(foo.isEmpty() == true);
        }

        TEST_METHOD(UnitTest_iaStringTest_85)
        {
            iaString foo("abc*fghi*jk");
            uint64 result = foo.findLastOf(L"XYZ");

            Assert::IsTrue(result == iaString::INVALID_POSITION);
        }

        TEST_METHOD(UnitTest_iaStringTest_86)
        {
            iaString foo("abc*fghi*jk");
            uint64 result = foo.findLastOf(L"Y");

            Assert::IsTrue(result == iaString::INVALID_POSITION);
        }

        TEST_METHOD(UnitTest_iaStringTest_87)
        {
            iaString foo("abc*fghi*jk");
            uint64 result = foo.findLastOf(L'Y');

            Assert::IsTrue(result == iaString::INVALID_POSITION);
        }

        TEST_METHOD(UnitTest_iaStringTest_88)
        {
            iaString *foo = new iaString();

            Assert::IsTrue(foo->getSize() == 0);
            Assert::IsTrue(foo->getData() == nullptr);
            Assert::IsTrue(foo->isEmpty() == true);

            delete foo;
        }

        TEST_METHOD(UnitTest_iaStringTest_89)
        {
            iaString foo("abc");
            iaString bar("abc");

            Assert::IsFalse(foo != bar);
        }

        TEST_METHOD(UnitTest_iaStringTest_90)
        {
            iaString foo("");
            iaString bar("");

            Assert::IsFalse(foo != bar);
        }

        TEST_METHOD(UnitTest_iaStringTest_91)
        {
            iaString foo;
            iaString bar;

            Assert::IsFalse(foo != bar);
        }

        TEST_METHOD(UnitTest_iaStringTest_92)
        {
            iaString foo("123");

            Assert::IsFalse(foo != "123");
        }

        TEST_METHOD(UnitTest_iaStringTest_93)
        {
            iaString foo("abc");
            iaString bar("abcd");

            Assert::IsTrue(foo != bar);
        }

        TEST_METHOD(UnitTest_iaStringTest_94)
        {
            const iaString foo("abc");

            Assert::IsTrue(foo[0] == 'a');
            Assert::IsTrue(foo[1] == 'b');
            Assert::IsTrue(foo[2] == 'c');
            Assert::IsTrue(foo.getData()[3] == 0);
            Assert::IsTrue(foo.getSize() != 0);
            Assert::IsTrue(foo.getData() != nullptr);
            Assert::IsTrue(foo.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_95)
        {
            const iaString foo("abc?");

            Assert::IsTrue(foo == "abc?");
            Assert::IsTrue(foo.getSize() != 0);
            Assert::IsTrue(foo.getData() != nullptr);
            Assert::IsTrue(foo.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_96)
        {
            iaString foo(u8"😇こんにちは世界");

            char buffer[100];
            uint16 size = foo.getUTF8Size();
            uint16 resultSize = foo.getUTF8(buffer, size);
            iaString bar;
            bar.setUTF8(buffer, size);

            Assert::IsTrue(size == 50);
            Assert::IsTrue(resultSize == size);
            Assert::IsTrue(foo == u8"😇こんにちは世界");
            Assert::IsTrue(foo == bar);
            Assert::IsTrue(foo.getSize() != 0);
            Assert::IsTrue(foo.getData() != nullptr);
            Assert::IsTrue(foo.isEmpty() == false);
        }

        TEST_METHOD(UnitTest_iaStringTest_97)
        {
            iaString foo(L"こんにちは世界");

            char buffer[100];
            uint16 size = foo.getUTF8Size();
            uint16 resultSize = foo.getUTF8(buffer, size);
            iaString bar;
            bar.setUTF8(buffer, size);

            Assert::IsTrue(size == 21);
            Assert::IsTrue(resultSize == size);
            Assert::IsTrue(foo == L"こんにちは世界");
            Assert::IsTrue(foo == bar);
            Assert::IsTrue(foo.getSize() != 0);
            Assert::IsTrue(foo.getData() != nullptr);
            Assert::IsTrue(foo.isEmpty() == false);
        }
    };

}