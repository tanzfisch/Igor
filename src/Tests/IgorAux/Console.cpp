#include <gtest/gtest.h>
#include <IgorAux.h>
#include <iaConsole.h>
using namespace IgorAux;

TEST(ConsoleTests, StartStop)
{
    startup();

    EXPECT_EQ(iaConsole::getInstance().getErrors(), 0);
    EXPECT_EQ(iaConsole::getInstance().getWarnings(), 0);
    EXPECT_EQ(iaConsole::getInstance().getLogLevel(), LogLevel::DebugInfo);

    shutdown();

    EXPECT_EQ(iaConsole::getInstance().getErrors(), 0);
    EXPECT_EQ(iaConsole::getInstance().getWarnings(), 0);
    EXPECT_EQ(iaConsole::getInstance().getLogLevel(), LogLevel::DebugInfo);
}

TEST(ConsoleTests, ErrorAndWarnings)
{
    startup();

    con_err("some error");
    con_warn("some warning");

    EXPECT_EQ(iaConsole::getInstance().getErrors(), 1);
    EXPECT_EQ(iaConsole::getInstance().getWarnings(), 1);
    EXPECT_EQ(iaConsole::getInstance().getLogLevel(), LogLevel::DebugInfo);

    shutdown();
}

TEST(ConsoleTests, SupressWarning)
{
    startup();

    iaConsole::getInstance().setLogLevel(LogLevel::Error);

    con_err("some error");
    con_warn("some warning");

    EXPECT_EQ(iaConsole::getInstance().getErrors(), 1);
    EXPECT_EQ(iaConsole::getInstance().getWarnings(), 0);
    EXPECT_EQ(iaConsole::getInstance().getLogLevel(), LogLevel::Error);

    shutdown();

    EXPECT_EQ(iaConsole::getInstance().getErrors(), 0);
    EXPECT_EQ(iaConsole::getInstance().getWarnings(), 0);
    EXPECT_EQ(iaConsole::getInstance().getLogLevel(), LogLevel::DebugInfo);
}

TEST(ConsoleTests, TestColors)
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

    EXPECT_EQ(iaConsole::getInstance().getErrors(), 0);
    EXPECT_EQ(iaConsole::getInstance().getWarnings(), 0);
    EXPECT_EQ(iaConsole::getInstance().getLogLevel(), LogLevel::DebugInfo);
}