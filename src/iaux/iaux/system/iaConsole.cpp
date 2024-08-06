// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/system/iaConsole.h>
#include <iaux/system/iaDirectory.h>
#include <iaux/system/iaSystem.h>
#include <iaux/system/iaThread.h>

#ifdef IGOR_WINDOWS
#include <windows.h>
#include <DbgHelp.h>
#endif

#ifdef IGOR_LINUX
#include <unistd.h>
#include <wchar.h>
#include <signal.h>
#endif

namespace iaux
{

#ifdef IGOR_WINDOWS
    // mapping der winapi farben auf die Igor Konsolen Farben
    WORD winapi_colors[] = {FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
                            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
                            FOREGROUND_BLUE | FOREGROUND_INTENSITY,
                            FOREGROUND_BLUE,
                            FOREGROUND_GREEN | FOREGROUND_INTENSITY,
                            FOREGROUND_GREEN,
                            FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
                            FOREGROUND_BLUE | FOREGROUND_GREEN,
                            FOREGROUND_RED | FOREGROUND_INTENSITY,
                            FOREGROUND_RED,
                            FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY,
                            FOREGROUND_BLUE | FOREGROUND_RED,
                            FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY,
                            FOREGROUND_GREEN | FOREGROUND_RED};

    HANDLE console_handle;
    CONSOLE_SCREEN_BUFFER_INFO console_info;
#endif

#ifdef IGOR_LINUX
    std::vector<std::wstring> linux_console_colors = {
        L"\x1B[97m", // White,
        L"\x1B[90m", // Gray
        L"\x1B[94m", // Blue
        L"\x1B[34m", // DarkBlue
        L"\x1B[92m", // Green
        L"\x1B[32m", // DarkGreen
        L"\x1B[96m", // Cyan
        L"\x1B[36m", // DarkCyan
        L"\x1B[91m", // Red
        L"\x1B[31m", // DarkRed
        L"\x1B[95m", // Magenta
        L"\x1B[35m", // DarkMagenta
        L"\x1B[93m", // Yellow
        L"\x1B[33m"  // DarkYellow
    };
#endif

    iaConsole::iaConsole()
    {
#ifdef IGOR_WINDOWS
        console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (console_handle)
        {
            GetConsoleScreenBufferInfo(console_handle, &console_info);
            _useColorsSupported = true;
        }
#endif

#ifdef IGOR_LINUX
        if (isatty(0) &&
            isatty(1) &&
            isatty(2))
        {
            _useColorsSupported = true;
        }
#endif
    }

    iaConsole &iaConsole::getInstance()
    {
        static iaConsole _instance;
        return _instance;
    }

    void iaConsole::closeLogfile()
    {
        if (_file.is_open())
        {
            _file.close();
        }
    }

    void iaConsole::setUseColors(bool useColors)
    {
        _useColors = useColors;
    }

    bool iaConsole::isUsingColors() const
    {
        return _useColors;
    }

    /*! Note: can't use classes iaDirectory or iaFile here because of possible deadlock in logging
     */
    void iaConsole::openLogfile()
    {
        if (_file.is_open())
        {
            return;
        }

        iaString logfilePath;

#ifdef IGOR_WINDOWS
#ifdef IGOR_MSCOMPILER
        wchar_t result[MAX_PATH];
        GetModuleFileName(NULL, result, MAX_PATH);
        std::wstring path = result;
        path = path.substr(0, path.find_last_of(IGOR_PATHSEPARATOR) + 1);
        path.append(L"\\igor.log");
        logfilePath = iaString(path.c_str());
        _file.open(path, std::fstream::out);
#endif

#ifdef IGOR_GCC
        logfilePath = L"igor.log";
        _file.open("igor.log", std::fstream::out);
#endif
#endif

#ifdef IGOR_LINUX
        logfilePath = L"/tmp/igor.log";
        _file.open("/tmp/igor.log", std::fstream::out);
#endif

        if (!_file.is_open())
        {
            con_err("can't open log file \"" << logfilePath << "\"");
        }
    }

    void iaConsole::exit()
    {
        printTombstone();

        *this << endlTab << iaForegroundColor::DarkRed << "Sorry Igor could not recover from that. Please feel free to send" << endlTab;
        *this << iaForegroundColor::DarkRed << "me the error log so I can improve Igor for you!" << endlTab;
        *this << iaForegroundColor::White << "Martin Loga (igorgameengine@protonmail.com)" << endl;

#ifdef IGOR_WINDOWS
        __debugbreak();
#endif

#ifdef IGOR_LINUX
        raise(SIGTRAP);
#endif

        std::exit(EXIT_FAILURE);
    }

    void iaConsole::setLogLevel(iaLogLevel logLevel)
    {
        _logLevel = logLevel;
    }

    std::wostream &operator<<(std::wostream &stream, const iaLogLevel &logLevel)
    {
        const static std::wstring text[] = {
            L"Fatal",
            L"Error",
            L"Warning",
            L"Info",
            L"User",
            L"Debug",
            L"Trace"};

        stream << text[static_cast<int>(logLevel)];
        return stream;
    }

    iaLogLevel iaConsole::getLogLevel() const
    {
        return _logLevel;
    }

    void iaConsole::printTombstone()
    {
        *this << endlTab;
        *this << iaForegroundColor::Gray << "     .-------." << endlTab;
        *this << iaForegroundColor::Gray << "   .'         `." << endlTab;
        *this << iaForegroundColor::Gray << "   |  " << iaForegroundColor::White << "R  I  P" << iaForegroundColor::Gray << "  |" << endlTab;
        *this << iaForegroundColor::Gray << "   |           |" << endlTab;
        *this << iaForegroundColor::Gray << "   |           |" << endlTab;
        *this << iaForegroundColor::Gray << "   | " << iaForegroundColor::Blue << "@" << iaForegroundColor::Gray << "         |" << endlTab;
        *this << iaForegroundColor::Green << " \\" << iaForegroundColor::Gray << " |" << iaForegroundColor::Green << "//       \\" << iaForegroundColor::Gray << " |" << iaForegroundColor::Green << "/" << endlTab;
        *this << iaForegroundColor::DarkGreen << "^^^^^^^^^^^^^^^^^^^" << endl;
    }

    void iaConsole::printCallStack(uint32 maxDepth)
    {
        std::vector<iaString> callStack;
        getCallStack(callStack);
        bool skip = true;
        for (int i = 1; i < callStack.size(); ++i)
        {
            *this << callStack[i];
            if (i != callStack.size() - 1)
            {
                *this << endlTab;
            }
        }
    }

    void iaConsole::printHeader(iaLogLevel logLevel)
    {
        iaForegroundColor color;
        iaString tag;

        switch (logLevel)
        {
        case iaLogLevel::Fatal:
            color = iaForegroundColor::Red;
            tag = " FATAL ";
            break;

        case iaLogLevel::Error:
            color = iaForegroundColor::DarkRed;
            tag = " ERROR ";
            break;

        case iaLogLevel::Warning:
            color = iaForegroundColor::Yellow;
            tag = "WARNING";
            break;

        case iaLogLevel::Info:
            color = iaForegroundColor::DarkBlue;
            tag = " INFO  ";
            break;

        case iaLogLevel::User:
            color = iaForegroundColor::Gray;
            tag = " USER  ";
            break;

        case iaLogLevel::Debug:
            color = iaForegroundColor::Gray;
            tag = " DEBUG ";
            break;

        case iaLogLevel::Trace:
            color = iaForegroundColor::Gray;
            tag = " TRACE ";
            break;
        }

        *this << iaForegroundColor::White << "IGOR " << iaForegroundColor::Gray
              << applicationTime << "|" << printIgorThreadID << iaForegroundColor::White
              << " [" << color << tag << iaForegroundColor::White << "] ";
    }

    void iaConsole::lock()
    {
        _mutex.lock();
    }

    void iaConsole::unlock()
    {
        _mutex.unlock();
    }

    void iaConsole::setTextColor(iaForegroundColor color)
    {
        if (_useColors && _useColorsSupported)
        {
#ifdef IGOR_WINDOWS
            if (console_handle)
            {
                if (color != iaForegroundColor::Gray)
                {
                    SetConsoleTextAttribute(console_handle, winapi_colors[static_cast<int>(color)]);
                }
                else
                {
                    SetConsoleTextAttribute(console_handle, console_info.wAttributes);
                }
            }
#endif

#ifdef IGOR_LINUX
            std::wcout << linux_console_colors[static_cast<int>(color)];
#endif
        }
    }

    uint32 iaConsole::getErrors()
    {
        return _errors;
    }

    uint32 iaConsole::getWarnings()
    {
        return _warnings;
    }

    void iaConsole::resetStats()
    {
        _errors = 0;
        _warnings = 0;
    }

    void iaConsole::printStats()
    {
        if (_errors != 0 || _warnings != 0)
        {
            *this << LOCK << iaForegroundColor::White << "IGOR " << iaForegroundColor::Gray
                  << applicationTime << "|" << printIgorThreadID << iaForegroundColor::White << " [ ----- ] "
                  << iaForegroundColor::Red << "Errors: " << _errors << iaForegroundColor::Yellow << " Warnings: " << _warnings << endl
                  << UNLOCK;
        }
        else
        {
            *this << LOCK << iaForegroundColor::White << "IGOR " << iaForegroundColor::Gray
                  << applicationTime << "|" << printIgorThreadID << iaForegroundColor::White << " [ ----- ] "
                  << iaForegroundColor::Green << "OK" << endl
                  << UNLOCK;
        }
    }

    void iaConsole::activateLogfile(bool activate)
    {
        _streamToLogfile = activate;
    }

    iaConsole &printIgorThreadID(iaConsole &console)
    {
        const iaID32 id = iaThread::getThisThreadID();
        console << (iaForegroundColor)((id-1) % 14) << std::setfill(L'0') << std::setw(3) << id << std::dec;
        return console;
    }

}; // namespace iaux