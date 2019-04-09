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
// (c) Copyright 2012-2019 by Martin Loga
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
// contact: martinloga@gmx.de  

#ifndef __IGOR_AUX_CONSOLE__
#define __IGOR_AUX_CONSOLE__

#include <iaSingleton.h>
#include <iaClock.h>

#ifdef __IGOR_WIN__
#include <windows.h>
#endif

#include <fstream>
#include <iostream>
#include <iomanip>
using namespace std;

// #define __IGOR_NO_ASSERTIONS__ uncomment the following line if you don't want any assertion checks not even the sticky ones.
// #define __IGOR_NO_ASSERTIONS__

// #define __IGOR_NO_DEBUGBREAK__ uncomment the following line if you want no debug break within an assertion
// #define __IGOR_NO_DEBUGBREAK__

namespace IgorAux
{
    class IgorAux_API iaConsole;

    /*!
    \todo forgot what this is good for
    */
    __IGOR_FUNCTION_POINTER__(superspecialfuncptrtype, __CLRCALL_OR_CDECL, iaConsole&, (iaConsole&));

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

    /*! console and logging interface
    */
    class IgorAux_API iaConsole : public iaSingleton<iaConsole>
    {
        friend class iaSingleton<iaConsole>;

        friend iaConsole& endl(iaConsole &console);
        friend iaConsole& flush(iaConsole &console);
        friend iaConsole& incerr(iaConsole &console);
        friend iaConsole& incwarn(iaConsole &console);
        friend iaConsole& LOCK(iaConsole &console);
        friend iaConsole& UNLOCK(iaConsole &console);
        friend iaConsole& printThreadID(iaConsole &console);
        friend iaConsole& generateWindowsError(iaConsole &console);
        friend iaConsole& applicationTime(iaConsole &console);

    public:

        /*! lock console for exclusive access
        */
        void lock();

        /*! unlock console
        */
        void unlock();

        /*! prints call stack

        \param depth max number of call stack depth to print
        */
        void printCallStack(uint32 maxDepth = 1000);

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
        template<typename T> iaConsole& operator << (const T &v)
        {
            wcout << v;
            if (_streamToLogfile && _file.is_open())
            {
                _file << v;
            }
            return *this;
        }

        /*!
        \todo forgot what this is good for
        */
        iaConsole& operator << (const superspecialfuncptrtype v)
        {
            v(*this);
            return *this;
        }

        /*! changes foreground color for output in console

        \param color foreground color
        */
        iaConsole& operator << (const iaForegroundColor color)
        {
            setTextColor(color);
            return *this;
        }

    private:

        /*! file stream to log file
        */
        wfstream _file;

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

#ifdef __IGOR_WIN__
#ifdef __IGOR_DEBUG__

    /*! works like the original assert

    will be fully removed in rlease build

    \param Condition a condition that returns false in case of an error
    \param Message additional message output
    */
#ifndef __IGOR_NO_ASSERTIONS__
#define con_assert(Condition, Message) \
    if (!(Condition)) \
    { \
        iaConsole::getInstance() << LOCK << iaForegroundColor::Gray << applicationTime << printThreadID << iaForegroundColor::Red << "ASSERTION " << iaForegroundColor::DarkRed << Message << " (" #Condition ")" << endl;\
        iaConsole::getInstance() << iaForegroundColor::DarkRed << __IGOR_LOGGING_TAB__ << __IGOR_FILE_LINE__ << endl; \
        iaConsole::getInstance() << iaForegroundColor::DarkRed << __IGOR_LOGGING_TAB__ << __IGOR_FUNCTION__ << endl; \
        iaConsole::getInstance() << iaForegroundColor::DarkRed << __IGOR_LOGGING_TAB__ << "-----------------------------------------------------------------------" << endl; \
        iaConsole::getInstance() << iaForegroundColor::DarkRed; \
        iaConsole::getInstance().printCallStack(); \
        iaConsole::getInstance().printTombstone(); \
        iaConsole::getInstance() << iaForegroundColor::Gray << UNLOCK; \
        iaConsole::getInstance().exit(); \
    }
#else
#define con_assert(Condition, Message)
#endif

    /*! works similar to an assert but opens the debugger instead of stopping the application in debug mode and stays in the release build

    \param Condition a condition that returns false in case of an error
    \param Message additional message output
    */
#ifndef __IGOR_NO_ASSERTIONS__
#define con_assert_sticky(Condition, Message) \
    if (!(Condition)) \
    { \
        iaConsole::getInstance() << LOCK << iaForegroundColor::Gray << applicationTime << printThreadID << iaForegroundColor::Red << "ASSERTION_DEBUG_BREAK " << iaForegroundColor::DarkRed << Message << " (" #Condition ")" << endl;\
        iaConsole::getInstance() << iaForegroundColor::DarkRed << __IGOR_LOGGING_TAB__ << __IGOR_FILE_LINE__ << endl; \
        iaConsole::getInstance() << iaForegroundColor::DarkRed << __IGOR_LOGGING_TAB__ << __IGOR_FUNCTION__ << endl; \
        iaConsole::getInstance() << iaForegroundColor::DarkRed << __IGOR_LOGGING_TAB__ << "-----------------------------------------------------------------------" << endl; \
        iaConsole::getInstance() << iaForegroundColor::DarkRed; \
        iaConsole::getInstance().printCallStack(10); \
        iaConsole::getInstance().printTombstone(); \
        iaConsole::getInstance() << iaForegroundColor::Gray << UNLOCK; \
        iaConsole::getInstance().exit(); \
    }
#else
#define con_assert_sticky(Condition, Message)
#endif

    /*! only called in debug mode

    \param Message message output
    */
#define con_debug(Message)\
    iaConsole::getInstance() << LOCK << iaForegroundColor::Gray << applicationTime << printThreadID << iaForegroundColor::Gray << Message << UNLOCK;

    /*! only called in debug mode

    including line feed

    \param Message message output
    */
#define con_debug_endl(Message)\
    iaConsole::getInstance() << LOCK << iaForegroundColor::Gray << applicationTime << printThreadID << iaForegroundColor::Gray << Message << endl << UNLOCK;

#else

#define con_assert(Condition, Message)

#ifndef __IGOR_NO_ASSERTIONS__
#define con_assert_sticky(Condition, Message) \
    if (!(Condition)) \
    { \
        iaConsole::getInstance() << LOCK << iaForegroundColor::Gray << applicationTime << printThreadID << iaForegroundColor::Red << "INTERNAL ERROR " << iaForegroundColor::DarkRed << Message << " (" #Condition ")" << endl;\
        iaConsole::getInstance() << iaForegroundColor::DarkRed << __IGOR_LOGGING_TAB__ << __IGOR_FILE_LINE__ << endl; \
        iaConsole::getInstance() << iaForegroundColor::DarkRed << __IGOR_LOGGING_TAB__ << __IGOR_FUNCTION__ << endl; \
        iaConsole::getInstance() << iaForegroundColor::DarkRed << __IGOR_LOGGING_TAB__ << "-----------------------------------------------------------------------" << endl; \
        iaConsole::getInstance() << iaForegroundColor::DarkRed; \
        iaConsole::getInstance().printCallStack(10); \
        iaConsole::getInstance() << endl << iaForegroundColor::Yellow << __IGOR_LOGGING_TAB__ << "Igor might not recover from an internal error. Please feel free to send" << endl; \
        iaConsole::getInstance() << iaForegroundColor::Yellow << __IGOR_LOGGING_TAB__         << "me the error log so I can improve Igor for you!" << iaForegroundColor::White << " martinloga@gmx.de" << endl; \
        iaConsole::getInstance() << iaForegroundColor::Gray << UNLOCK; \
        iaConsole::getInstance().exit(); \
    } 
#else
#define con_assert_sticky(Condition, Message)
#endif

#define con_debug(Message)
#define con_debug_endl(Message)

#endif

    /*! prints an error message to console and optionally to the log file

    \param Message message to be printed
    */
#define con_err(Message) \
    iaConsole::getInstance() << LOCK << iaForegroundColor::Gray << applicationTime << printThreadID << iaForegroundColor::Red << incerr << "ERROR " << Message << endl; \
    iaConsole::getInstance() << iaForegroundColor::DarkRed << __IGOR_LOGGING_TAB__ << __IGOR_FILE_LINE__ << endl; \
    iaConsole::getInstance() << iaForegroundColor::DarkRed << __IGOR_LOGGING_TAB__ << __IGOR_FUNCTION__ << endl; \
    iaConsole::getInstance() << iaForegroundColor::DarkRed; \
    iaConsole::getInstance().printCallStack(10); \
    iaConsole::getInstance() << iaForegroundColor::Gray << UNLOCK; \
    con_assert_sticky(iaConsole::getInstance().getErrors() < 100, "too many errors")

    /*! prints an windows specific error message to console and optionally to the log file

    \param Message message to be printed
    */
#define con_err_win(Message) \
    iaConsole::getInstance() << LOCK << iaForegroundColor::Gray << applicationTime << printThreadID << iaForegroundColor::Red << incerr << "ERROR " << Message << endl; \
    iaConsole::getInstance() << iaForegroundColor::DarkRed << __IGOR_LOGGING_TAB__ << __IGOR_FILE_LINE__ << endl; \
    iaConsole::getInstance() << iaForegroundColor::DarkRed << __IGOR_LOGGING_TAB__ << __IGOR_FUNCTION__ << endl;\
    iaConsole::getInstance() << iaForegroundColor::DarkRed << __IGOR_LOGGING_TAB__ << endl; \
    iaConsole::getInstance() << generateWindowsError << endl; \
    iaConsole::getInstance() << iaForegroundColor::DarkRed; \
    iaConsole::getInstance().printCallStack(10); \
    iaConsole::getInstance() << iaForegroundColor::Gray << UNLOCK; \
    con_assert_sticky(iaConsole::getInstance().getErrors() < 100, "too many errors")

    /*! prints an warning message to console and optionally to the log file

    \param Message message to be printed
    */
#define con_warn(Message) \
    iaConsole::getInstance() << LOCK << iaForegroundColor::Gray << applicationTime << printThreadID << iaForegroundColor::Yellow << incwarn << "WARNING " << Message << endl; \
    iaConsole::getInstance() << iaForegroundColor::DarkYellow << __IGOR_LOGGING_TAB__ << __IGOR_FILE_LINE__ << endl; \
    iaConsole::getInstance() << iaForegroundColor::DarkYellow << __IGOR_LOGGING_TAB__ << __IGOR_FUNCTION__ << endl; \
    iaConsole::getInstance() << iaForegroundColor::Gray << UNLOCK; \
    con_assert_sticky(iaConsole::getInstance().getWarnings() < 100, "too many warnings")

    /*! prints an info message to console and optionally to the log file

    \param Type type of information to be printed
    \param Message message to be printed
	\todo would be nice to have a fixed size of info type collumn
    */
#define con_info(Type, Message) \
    iaConsole::getInstance() << LOCK << iaForegroundColor::Gray << applicationTime << printThreadID <<  iaForegroundColor::Cyan << Type << iaForegroundColor::Gray << " - " << iaForegroundColor::DarkCyan << Message << endl << UNLOCK;

    /*! prints an message to console and optionally to the log file

    \param Message message to be printed
    */
#define con(Message)\
    iaConsole::getInstance() << LOCK << Message << UNLOCK;

    /*! prints an message to console and optionally to the log file.
    In addition it prints an end line at the end.

    \param Message message to be printed
    */
#define con_endl(Message)\
    iaConsole::getInstance() << LOCK << iaForegroundColor::Gray << applicationTime << printThreadID << iaForegroundColor::Gray << Message << endl << UNLOCK;

#endif

    /*! generated a windows specific error message and pipes it in to the console

    \param console console instance to use
    \returns the console instance
    */
    __IGOR_INLINE__ iaConsole& generateWindowsError(iaConsole &console)
    {
        int error = GetLastError();

        if (error != 0)
        {
            char * errorString = 0;
            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                0, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)& errorString, 0, 0);

            if (errorString)
            {
                console << errorString;
                if (console._streamToLogfile && console._file.is_open())
                {
                    console._file << errorString;
                }
                LocalFree(errorString);
            }
        }
        else
        {
            console << "no error code";
            if (console._streamToLogfile && console._file.is_open())
            {
                console._file << "no error code";
            }
        }

        return console;
    }

    __IGOR_INLINE__ iaConsole& applicationTime(iaConsole &console)
    {
        uint64 time = static_cast<uint64>(iaClock::getClockMiliseconds());
        uint64 seconds = (time / 1000) % 60;
        uint64 minutes = (time / 1000 * 60) % 60;
        uint64 hours = (time / 1000 * 60 * 60) % 24;
        console << setfill(L'0') << setw(2) << hours << ":";
        console << setfill(L'0') << setw(2) << minutes << ":";
        console << setfill(L'0') << setw(2) << seconds << ":";
        console << setfill(L'0') << setw(3) << time % 1000 << " ";
        return console;
    }

    /*! prints an endline to the console

    \param console console instance to use
    \returns the console instance
    */
    __IGOR_INLINE__ iaConsole& endl(iaConsole &console)
    {
        iaConsole::getInstance().setTextColor(iaForegroundColor::Gray);
        wcout << std::endl;
        if (console._streamToLogfile && console._file.is_open())
        {
            console._file << std::endl;
        }
        return console;
    }

    /*! sends an flush to the console

    \param console console instance to use
    \returns the console instance
    */
    __IGOR_INLINE__ iaConsole& flush(iaConsole &console)
    {
        iaConsole::getInstance().setTextColor(iaForegroundColor::Gray);
        wcout << std::flush;
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
    __IGOR_INLINE__ iaConsole& incerr(iaConsole &console)
    {
        console._errors++;
        return console;
    }

    /*! increments the warning counter

    \param console console instance to use
    \returns the console instance
    */
    __IGOR_INLINE__ iaConsole& incwarn(iaConsole &console)
    {
        console._warnings++;
        return console;
    }

    /*! locks mutex

    \param console console instance to use
    \returns the console instance
    */
    __IGOR_INLINE__ iaConsole& LOCK(iaConsole &console)
    {
        console.lock();
        return console;
    }

    /*! unlocks mutex

    \param console console instance to use
    \returns the console instance
    */
    __IGOR_INLINE__ iaConsole& UNLOCK(iaConsole &console)
    {
        console.unlock();
        return console;
    }

    __IGOR_INLINE__ iaConsole& printThreadID(iaConsole &console)
    {
        int32 threadID = GetCurrentThreadId();
        console << "[" << setfill(L'0') << setw(8) << hex << threadID << dec << "] ";
        return console;
    }

};

#endif

