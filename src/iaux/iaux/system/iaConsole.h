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

#ifndef __IAUX_CONSOLE_H__
#define __IAUX_CONSOLE_H__

#include <iaux/system/iaSingleton.h>
#include <iaux/system/iaClock.h>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <thread>
#include <map>

namespace iaux
{
    class IgorAux_API iaConsole;

/*! logging tab definition including size of time and thread ID
    */
#define __IGOR_LOGGING_TAB__ L"                             "

    /*!
    \todo forgot what this is good for
    */
    __IGOR_FUNCTION_POINTER__(superspecialfuncptrtype, iaConsole &, (iaConsole &));

    /*! used as foreground colors for console output
    */
    enum class iaForegroundColor
    {
        White,
        Gray,
        Blue,
        DarkBlue,
        Green,
        DarkGreen,
        Cyan,
        DarkCyan,
        Red,
        DarkRed,
        Magenta,
        DarkMagenta,
        Yellow,
        DarkYellow
    };

    /*! logging level definitions
    */
    enum class iaLogLevel
    {
        /*! this would make the application stop

        con_assert, con_assert_sticky
        */
        Fatal,

        /*! there is a problem and the application might or moght not be able to deal with it

        con_err
        */
        Error,

        /*! there is a minor problem and you should fix it

        con_warn
        */
        Warning,

        /*! informational output about what the engine does

        con_info
        */
        Info,

        /*! it's stuff somewhere between info and what the engine would log when actually compiled for debug.
        It's the equivalent of a plain cout or printf.

        con, con_endl
        */
        DebugInfo, // TODO need better name for this... or we can put it together with Info

        /*! debug output

        con_debug, con_debug_endl
        */
        Debug,

        /*! trace everything that there is

        con_trace
        */
        Trace
    };

    /*! console and logging interface
    */
    class IgorAux_API iaConsole : public iaSingleton<iaConsole>
    {
        friend class iaSingleton<iaConsole>;
        friend void *threadFunc(void *data);

        friend iaConsole &endl(iaConsole &console);
        friend iaConsole &endlTab(iaConsole &console);
        friend iaConsole &flush(iaConsole &console);
        friend iaConsole &incerr(iaConsole &console);
        friend iaConsole &incwarn(iaConsole &console);
        friend iaConsole &LOCK(iaConsole &console);
        friend iaConsole &UNLOCK(iaConsole &console);
        friend iaConsole &printThreadID(iaConsole &console);
        friend iaConsole &applicationTime(iaConsole &console);

    public:
        /*! lock console for exclusive access
        */
        void lock();

        /*! unlock console
        */
        void unlock();

        /*! sets the log level

        \param logLevel the log level
        */
        void setLogLevel(iaLogLevel logLevel);

        /*! \returns the current log level
        */
        iaLogLevel getLogLevel() const;

        /*! prints call stack

        \param depth max number of call stack depth to print
        */
        void printCallStack(uint32 maxDepth = 1000);

        /*! prints logging head

        \param logLevel represents the tag used in the head
        */
        void printHead(iaLogLevel logLevel);

        /*! activates or deactivates log file output

        \param activate true: activates log file output; false: console output only
        */
        void activateLogfile(bool activate);

        /*! prints warning and error counter
        */
        void printStats();

        /*! prints a tombstone in the log
        */
        void printTombstone();

        /*! exits application or calls debug break
        */
        void exit();

        /*! prints a birthday cake in the log
        */
        void printCake();

        /*! reset error and warning counter
        */
        void resetStats();

        /*! \returns error counter
        */
        uint32 getErrors();

        /*! \returns warning counter
        */
        uint32 getWarnings();

        /*! manually initializing the log file
        */
        void openLogfile();

        /*! closes logging to log file
        */
        void closeLogfile();

        /*! pipes anything in to console and log
        */
        template <typename T>
        iaConsole &operator<<(const T &v)
        {
            std::wcout << v;
            if (_streamToLogfile && _file.is_open())
            {
                _file << v;
            }
            return *this;
        }

        /*!
        \todo forgot what this is good for
        */
        iaConsole &operator<<(const superspecialfuncptrtype v)
        {
            v(*this);
            return *this;
        }

        /*! changes foreground color for output in console

        \param color foreground color
        */
        iaConsole &operator<<(const iaForegroundColor color)
        {
            setTextColor(color);
            return *this;
        }

        /*! sets wether or not to use colors for console output

        \param useColors if true using colors for console output
        */
        void setUseColors(bool useColors = true);

        /*! \returns true if using colors for console output
        */
        bool isUsingColors() const;

    private:
        /*! the log level. default is logging everything (trace)
        */
        iaLogLevel _logLevel = iaLogLevel::Trace;

        /*! file stream to log file
        */
        std::wfstream _file;

        /*! true: log to file is active; false: log to file will be scipped

        but there is always console output
        */
        bool _streamToLogfile = true;

        /*! warning counter
        */
        uint32 _warnings = 0;

        /*! error counter
        */
        uint32 _errors = 0;

        /*! mutex for multithreded access
        */
        iaMutex _mutex;

        /*! if true use colors
        */
        bool _useColors = true;

        /*! if true use of colors is supported
        */
        bool _useColorsSupported = false;

        /*! maps thread IDs to thread names
        */
        std::map<size_t, iaID32> _threadIDs;

        /*! changes foreground color of the console text

        \param color foreground color
        */
        void setTextColor(iaForegroundColor color);

        /*! ctor

        initialize console access
        open log for write
        */
        iaConsole();

        /*! nothing to do. the console is not meant to be destructed
        */
        virtual ~iaConsole() = default;
    };

#ifdef __IGOR_DEBUG__
    /*! works like the original assert

    will be fully removed in rlease build

    \param Condition a condition that returns false in case of an error
    \param Message additional message output
    */
#define con_assert(Condition, Message)                                                                                    \
    if (iaConsole::getInstance().getLogLevel() >= iaLogLevel::Fatal && !(Condition))                                      \
    {                                                                                                                     \
        iaConsole::getInstance() << LOCK;                                                                                 \
        iaConsole::getInstance().printHead(iaLogLevel::Fatal);                                                            \
        iaConsole::getInstance() << iaForegroundColor::DarkRed << Message << " (" #Condition ")" << endlTab;              \
        iaConsole::getInstance() << __IGOR_FILE_LINE__ << endlTab;                                                        \
        iaConsole::getInstance() << __IGOR_FUNCTION__ << endlTab;                                                         \
        iaConsole::getInstance() << "-----------------------------------------------------------------------" << endlTab; \
        iaConsole::getInstance().printCallStack();                                                                        \
        iaConsole::getInstance() << UNLOCK;                                                                               \
        iaConsole::getInstance().exit();                                                                                  \
    }

    /*! works similar to an assert but opens the debugger instead of stopping the application in debug mode and stays in the release build

    \param Condition a condition that returns false in case of an error
    \param Message additional message output
    */
#define con_assert_sticky(Condition, Message)                                                                             \
    if (iaConsole::getInstance().getLogLevel() >= iaLogLevel::Fatal && !(Condition))                                      \
    {                                                                                                                     \
        iaConsole::getInstance() << LOCK;                                                                                 \
        iaConsole::getInstance().printHead(iaLogLevel::Fatal);                                                            \
        iaConsole::getInstance() << iaForegroundColor::DarkRed << Message << " (" #Condition ")" << endlTab;              \
        iaConsole::getInstance() << __IGOR_FILE_LINE__ << endlTab;                                                        \
        iaConsole::getInstance() << __IGOR_FUNCTION__ << endlTab;                                                         \
        iaConsole::getInstance() << "-----------------------------------------------------------------------" << endlTab; \
        iaConsole::getInstance().printCallStack(10);                                                                      \
        iaConsole::getInstance() << UNLOCK;                                                                               \
        iaConsole::getInstance().exit();                                                                                  \
    }

    /*! only called in debug mode

    including line feed

    \param Message message output
    */
#define con_debug_endl(Message)                                      \
    if (iaConsole::getInstance().getLogLevel() >= iaLogLevel::Debug) \
    {                                                                \
        iaConsole::getInstance() << LOCK;                            \
        iaConsole::getInstance().printHead(iaLogLevel::Debug);       \
        iaConsole::getInstance() << Message << endl                  \
                                 << UNLOCK;                          \
    }

    /*! only called in debug mode

        including line feed

        \param Message message output
        */
#define con_trace()                                                                                                          \
    if (iaConsole::getInstance().getLogLevel() >= iaLogLevel::Trace)                                                         \
    {                                                                                                                        \
        iaConsole::getInstance() << LOCK;                                                                                    \
        iaConsole::getInstance().printHead(iaLogLevel::Trace);                                                               \
        iaConsole::getInstance() << iaForegroundColor::DarkMagenta << __IGOR_FUNCTION__ << " " << __IGOR_FILE_LINE__ << endl \
                                 << UNLOCK;                                                                                  \
    }

#else // __IGOR_DEBUG__

#define con_assert(Condition, Message)

#define con_assert_sticky(Condition, Message)                                                                             \
    if (iaConsole::getInstance().getLogLevel() >= iaLogLevel::Fatal && !(Condition))                                      \
    {                                                                                                                     \
        iaConsole::getInstance() << LOCK;                                                                                 \
        iaConsole::getInstance().printHead(iaLogLevel::Fatal);                                                            \
        iaConsole::getInstance() << iaForegroundColor::DarkRed << Message << " (" #Condition ")" << endlTab;              \
        iaConsole::getInstance() << __IGOR_FILE_LINE__ << endlTab;                                                        \
        iaConsole::getInstance() << __IGOR_FUNCTION__ << endlTab;                                                         \
        iaConsole::getInstance() << "-----------------------------------------------------------------------" << endlTab; \
        iaConsole::getInstance().printCallStack(10);                                                                      \
        iaConsole::getInstance() << endl                                                                                  \
                                 << UNLOCK;                                                                               \
        iaConsole::getInstance().exit();                                                                                  \
    }

#define con_debug_endl(Message)
#define con_trace()

#endif // __IGOR_DEBUG__

    /*! prints an error message to console and optionally to the log file

        \param Message message to be printed
        */
#define con_err(Message)                                                                        \
    if (iaConsole::getInstance().getLogLevel() >= iaLogLevel::Error)                            \
    {                                                                                           \
        iaConsole::getInstance() << LOCK;                                                       \
        iaConsole::getInstance().printHead(iaLogLevel::Error);                                  \
        iaConsole::getInstance() << incerr << iaForegroundColor::DarkRed << Message << endlTab; \
        iaConsole::getInstance() << __IGOR_FILE_LINE__ << endlTab;                              \
        iaConsole::getInstance() << __IGOR_FUNCTION__ << endlTab;                               \
        iaConsole::getInstance().printCallStack(10);                                            \
        iaConsole::getInstance() << endl                                                        \
                                 << UNLOCK;                                                     \
        con_assert_sticky(iaConsole::getInstance().getErrors() < 100, "too many errors");       \
    }

    /*! prints an warning message to console and optionally to the log file

        \param Message message to be printed
        */
#define con_warn(Message)                                                                           \
    if (iaConsole::getInstance().getLogLevel() >= iaLogLevel::Warning)                              \
    {                                                                                               \
        iaConsole::getInstance() << LOCK;                                                           \
        iaConsole::getInstance().printHead(iaLogLevel::Warning);                                    \
        iaConsole::getInstance() << incwarn << iaForegroundColor::DarkYellow << Message << endlTab; \
        iaConsole::getInstance() << __IGOR_FILE_LINE__ << endlTab;                                  \
        iaConsole::getInstance() << __IGOR_FUNCTION__ << endl;                                      \
        iaConsole::getInstance() << UNLOCK;                                                         \
        con_assert_sticky(iaConsole::getInstance().getWarnings() < 200, "too many warnings");       \
    }

    /*! prints an info message to console and optionally to the log file

            \param Message message to be printed
            \todo would be nice to have a fixed size of info type collumn
            */
#define con_info(Message)                                                          \
    if (iaConsole::getInstance().getLogLevel() >= iaLogLevel::Info)                \
    {                                                                              \
        iaConsole::getInstance() << LOCK;                                          \
        iaConsole::getInstance().printHead(iaLogLevel::Info);                      \
        iaConsole::getInstance() << iaForegroundColor::DarkCyan << Message << endl \
                                 << UNLOCK;                                        \
    }

    /*! prints an message to console and optionally to the log file.
            In addition it prints an end line at the end.

            \param Message message to be printed
            */
#define con_endl(Message)                                                      \
    if (iaConsole::getInstance().getLogLevel() >= iaLogLevel::DebugInfo)       \
    {                                                                          \
        iaConsole::getInstance() << LOCK;                                      \
        iaConsole::getInstance().printHead(iaLogLevel::DebugInfo);             \
        iaConsole::getInstance() << iaForegroundColor::Gray << Message << endl \
                                 << UNLOCK;                                    \
    }

    /*! prints an endline to the console

    \param console console instance to use
    \returns the console instance
    */
    __IGOR_INLINE__ iaConsole &endl(iaConsole &console)
    {
        iaConsole::getInstance().setTextColor(iaForegroundColor::Gray);
        std::wcout << std::endl;
        if (console._streamToLogfile && console._file.is_open())
        {
            console._file << std::endl;
        }
        return console;
    }

    /*! prints an endline and a logging tab to the console

    \param console console instance to use
    \returns the console instance
    */
    __IGOR_INLINE__ iaConsole &endlTab(iaConsole &console)
    {
        std::wcout << std::endl
                   << __IGOR_LOGGING_TAB__;
        if (console._streamToLogfile && console._file.is_open())
        {
            console._file << std::endl
                          << __IGOR_LOGGING_TAB__;
        }
        return console;
    }

    /*! sends an flush to the console

    \param console console instance to use
    \returns the console instance
    */
    __IGOR_INLINE__ iaConsole &flush(iaConsole &console)
    {
        iaConsole::getInstance().setTextColor(iaForegroundColor::Gray);
        std::wcout << std::flush;
        if (console._streamToLogfile && console._file.is_open())
        {
            console._file << std::flush;
        }
        return console;
    }

    /*! increments the error counter

    \param console console instance to use
    \returns the console instance
    */
    __IGOR_INLINE__ iaConsole &incerr(iaConsole &console)
    {
        console._errors++;
        return console;
    }

    /*! increments the warning counter

    \param console console instance to use
    \returns the console instance
    */
    __IGOR_INLINE__ iaConsole &incwarn(iaConsole &console)
    {
        console._warnings++;
        return console;
    }

    /*! locks mutex

    \param console console instance to use
    \returns the console instance
    */
    __IGOR_INLINE__ iaConsole &LOCK(iaConsole &console)
    {
        console.lock();
        return console;
    }

    /*! unlocks mutex

    \param console console instance to use
    \returns the console instance
    */
    __IGOR_INLINE__ iaConsole &UNLOCK(iaConsole &console)
    {
        console.unlock();
        return console;
    }

    /*! prints this thread id in the console

    \param console console instance to use
    \returns the console instance
    */
    __IGOR_INLINE__ iaConsole &printThreadID(iaConsole &console)
    {
        uint32 id = 0;
        std::hash<std::thread::id> hashFunc;
        size_t threadID = hashFunc(std::this_thread::get_id());

        auto iter = console._threadIDs.find(threadID);
        if (iter != console._threadIDs.end())
        {
            id = iter->second;
        }

        console << std::setfill(L'0') << std::setw(2) << std::hex << id << std::dec << " ";
        return console;
    }

    /*! prints application time in the console

    \param console console instance to use
    \returns the console instance
    */
    __IGOR_INLINE__ iaConsole &applicationTime(iaConsole &console)
    {
        uint64 time = static_cast<uint64>(iaClock::getTimeMilliseconds());
        uint64 seconds = (time / 1000) % 60;
        uint64 minutes = (time / 1000 * 60) % 60;
        uint64 hours = (time / 1000 * 60 * 60) % 100; // show up to 99 h
        console << std::setfill(L'0') << std::setw(2) << hours << ":";
        console << std::setfill(L'0') << std::setw(2) << minutes << ":";
        console << std::setfill(L'0') << std::setw(2) << seconds << ":";
        console << std::setfill(L'0') << std::setw(3) << time % 1000 << " "; // ms
        return console;
    }

    /*! prints the log level in the console

    \param logLevel the log level to print
    */
    iaConsole &operator<<(iaConsole &console, const iaLogLevel &logLevel);

}; // namespace iaux

#endif // __IAUX_CONSOLE_H__
