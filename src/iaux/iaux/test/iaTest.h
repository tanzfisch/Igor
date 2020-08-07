//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2020 by Martin Loga
//
// This library is free software; you can redistribute it and or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.
//
// contact: igorgameengine@protonmail.com

#ifndef __IAUX_TEST_H__
#define __IAUX_TEST_H__

#include <iaux/system/iaConsole.h>

#include <string>
#include <map>
#include <vector>
#include <iostream>

namespace iaux
{
    /*! igor aux testing
    */
    class IAUX_API iaTest
    {
    public:
        /*! run individual test
        */
        virtual void run() = 0;

        /*! \returns group name of test
        */
        virtual const char *getGroupName() = 0;

        /*! \returns name of test
        */
        virtual const char *getName() = 0;

        /*! \returns location of test in code
        */
        virtual const char *getLocation() = 0;

        /*! initializes testing

        \param argc cli argc
        \param argv cli argv
        */
        static void initTests(int argc, char **argv);

        /*! runs the tests according to initialisation parameters
        */
        static void runTests();

        /*! \returns true if this individual test was successful
        */
        bool success()
        {
            return _ok;
        }

    protected:
        /*! registers individual test to testing list

        \param test the test to add
        */
        static void registerTest(iaTest *test);

        /*! the list of registred tests
        */
        static std::map<std::string, std::vector<iaTest *>> _tests;

        /*! if true this test was successful
        */
        bool _ok = true;
    };

#define IAUX_TEST(testGroup, testName)                                                        \
    class testGroup##_##testName : public iaTest                                              \
    {                                                                                         \
    public:                                                                                   \
        testGroup##_##testName() { registerTest(this); };                                     \
        const char *getGroupName() override                                                   \
        {                                                                                     \
            return #testGroup;                                                                \
        }                                                                                     \
        const char *getName() override                                                        \
        {                                                                                     \
            return #testName;                                                                 \
        }                                                                                     \
        const char *getLocation() override                                                    \
        {                                                                                     \
            return __IGOR_FILE_LINE__;                                                        \
        }                                                                                     \
        void run() override;                                                                  \
    };                                                                                        \
    testGroup##_##testName *testGroup##_##testName##_instance = new testGroup##_##testName(); \
    void testGroup##_##testName::run()

#define IAUX_EXPECT_EQ(a, b)                                                   \
    if ((a) != (b))                                                            \
    {                                                                          \
        iaConsole::getInstance() << (a) << " is not equal to " << (b) << endl; \
        iaConsole::getInstance() << __IGOR_FILE_LINE__ << endl;                \
        _ok = false;                                                           \
    }

#define IAUX_EXPECT_NE(a, b)                                                          \
    if ((a) == (b))                                                                   \
    {                                                                                 \
        iaConsole::getInstance() << (a) << " should be not equal to " << (b) << endl; \
        iaConsole::getInstance() << __IGOR_FILE_LINE__ << endl;                       \
        _ok = false;                                                                  \
    }

#define IAUX_EXPECT_TRUE(value)                                                  \
    if (!(value))                                                                \
    {                                                                            \
        iaConsole::getInstance() << "value should be TRUE but is FALSE" << endl; \
        iaConsole::getInstance() << __IGOR_FILE_LINE__ << endl;                  \
        _ok = false;                                                             \
    }

#define IAUX_EXPECT_FALSE(value)                                                 \
    if (value)                                                                   \
    {                                                                            \
        iaConsole::getInstance() << "value should be FALSE but is TRUE" << endl; \
        iaConsole::getInstance() << __IGOR_FILE_LINE__ << endl;                  \
        _ok = false;                                                             \
    }

#define IAUX_EXPECT_NEAR(a, b, t)                                                                                                                           \
    if (std::abs((a) - (b)) > t)                                                                                                                            \
    {                                                                                                                                                       \
        iaConsole::getInstance() << "diff of " << a << " and " << b << " is " << std::abs((a) - (b)) << " but should be smaller or equal to " << t << endl; \
        iaConsole::getInstance() << __IGOR_FILE_LINE__ << endl;                                                                                             \
        _ok = false;                                                                                                                                        \
    }

#define IAUX_EXPECT_GREATER_THEN(a, b)                                         \
    if ((a) <= (b))                                                            \
    {                                                                          \
        iaConsole::getInstance() << a << " is not greater then " << b << endl; \
        iaConsole::getInstance() << __IGOR_FILE_LINE__ << endl;                \
        _ok = false;                                                           \
    }

#define IAUX_EXPECT_LESS_THEN(a, b)                                         \
    if ((a) >= (b))                                                         \
    {                                                                       \
        iaConsole::getInstance() << a << " is not less then " << b << endl; \
        iaConsole::getInstance() << __IGOR_FILE_LINE__ << endl;             \
        _ok = false;                                                        \
    }

#define IAUX_COMPARE_MATRIX(m1, m2)                          \
    IAUX_EXPECT_NEAR(m1._pos._x, m2._pos._x, 0.0000001);     \
    IAUX_EXPECT_NEAR(m1._pos._y, m2._pos._y, 0.0000001);     \
    IAUX_EXPECT_NEAR(m1._pos._z, m2._pos._z, 0.0000001);     \
    IAUX_EXPECT_NEAR(m1._w0, m2._w0, 0.0000001);             \
    IAUX_EXPECT_NEAR(m1._right._x, m2._right._x, 0.0000001); \
    IAUX_EXPECT_NEAR(m1._right._y, m2._right._y, 0.0000001); \
    IAUX_EXPECT_NEAR(m1._right._z, m2._right._z, 0.0000001); \
    IAUX_EXPECT_NEAR(m1._w1, m2._w1, 0.0000001);             \
    IAUX_EXPECT_NEAR(m1._top._x, m2._top._x, 0.0000001);     \
    IAUX_EXPECT_NEAR(m1._top._y, m2._top._y, 0.0000001);     \
    IAUX_EXPECT_NEAR(m1._top._z, m2._top._z, 0.0000001);     \
    IAUX_EXPECT_NEAR(m1._w2, m2._w2, 0.0000001);             \
    IAUX_EXPECT_NEAR(m1._depth._x, m2._depth._x, 0.0000001); \
    IAUX_EXPECT_NEAR(m1._depth._y, m2._depth._y, 0.0000001); \
    IAUX_EXPECT_NEAR(m1._depth._z, m2._depth._z, 0.0000001); \
    IAUX_EXPECT_NEAR(m1._w3, m2._w3, 0.0000001)

} // namespace iaux

#endif // __IAUX_TEST_H__
