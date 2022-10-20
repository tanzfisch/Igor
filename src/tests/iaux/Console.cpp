#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>
#include <iaux/system/iaConsole.h>
using namespace iaux;

IAUX_TEST(ConsoleTests, StartStop)
{
    startup();

    IAUX_EXPECT_EQUAL(iaConsole::getInstance().getErrors(), 0);
    IAUX_EXPECT_EQUAL(iaConsole::getInstance().getWarnings(), 0);
    IAUX_EXPECT_EQUAL(iaConsole::getInstance().getLogLevel(), iaLogLevel::User);

    shutdown();

    IAUX_EXPECT_EQUAL(iaConsole::getInstance().getErrors(), 0);
    IAUX_EXPECT_EQUAL(iaConsole::getInstance().getWarnings(), 0);
    IAUX_EXPECT_EQUAL(iaConsole::getInstance().getLogLevel(), iaLogLevel::User);
}

IAUX_TEST(ConsoleTests, ErrorAndWarnings)
{
    startup();

    con_err("some error");
    con_warn("some warning");

    IAUX_EXPECT_EQUAL(iaConsole::getInstance().getErrors(), 1);
    IAUX_EXPECT_EQUAL(iaConsole::getInstance().getWarnings(), 1);
    IAUX_EXPECT_EQUAL(iaConsole::getInstance().getLogLevel(), iaLogLevel::User);

    shutdown();
}

IAUX_TEST(ConsoleTests, SupressWarning)
{
    startup();

    iaConsole::getInstance().setLogLevel(iaLogLevel::Error);

    con_err("some error");
    con_warn("some warning");

    IAUX_EXPECT_EQUAL(iaConsole::getInstance().getErrors(), 1);
    IAUX_EXPECT_EQUAL(iaConsole::getInstance().getWarnings(), 0);
    IAUX_EXPECT_EQUAL(iaConsole::getInstance().getLogLevel(), iaLogLevel::Error);

    shutdown();

    IAUX_EXPECT_EQUAL(iaConsole::getInstance().getErrors(), 0);
    IAUX_EXPECT_EQUAL(iaConsole::getInstance().getWarnings(), 0);
    IAUX_EXPECT_EQUAL(iaConsole::getInstance().getLogLevel(), iaLogLevel::User);
}

IAUX_TEST(ConsoleTests, TestColors)
{
    startup();

    con_endl(iaForegroundColor::White << "White");
    con_endl(iaForegroundColor::Gray << "Gray");
    con_endl(iaForegroundColor::Blue << "Blue");
    con_endl(iaForegroundColor::DarkBlue << "DarkBlue");
    con_endl(iaForegroundColor::Green << "Green");
    con_endl(iaForegroundColor::DarkGreen << "DarkGreen");
    con_endl(iaForegroundColor::Cyan << "Cyan");
    con_endl(iaForegroundColor::DarkCyan << "DarkCyan");
    con_endl(iaForegroundColor::Red << "Red");
    con_endl(iaForegroundColor::DarkRed << "DarkRed");
    con_endl(iaForegroundColor::Magenta << "Magenta");
    con_endl(iaForegroundColor::DarkMagenta << "DarkMagenta");
    con_endl(iaForegroundColor::Yellow << "Yellow");
    con_endl(iaForegroundColor::DarkYellow << "DarkYellow");

    shutdown();

    IAUX_EXPECT_EQUAL(iaConsole::getInstance().getErrors(), 0);
    IAUX_EXPECT_EQUAL(iaConsole::getInstance().getWarnings(), 0);
    IAUX_EXPECT_EQUAL(iaConsole::getInstance().getLogLevel(), iaLogLevel::User);
}