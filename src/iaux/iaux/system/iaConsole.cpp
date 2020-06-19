// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/system/iaConsole.h>
#include <iaux/system/iaDirectory.h>
#include <iaux/system/iaSystem.h>

#ifdef __IGOR_WINDOWS__
#include <windows.h>
#include <DbgHelp.h>
#endif

#ifdef __IGOR_LINUX__
#include <unistd.h>
#include <wchar.h>
#endif

namespace iaux
{

#ifdef __IGOR_WINDOWS__
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

#ifdef __IGOR_LINUX__
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
        // assuming this is the main thread
        std::hash<std::thread::id> hashFunc;
        size_t hashValue = hashFunc(std::this_thread::get_id());
        _threadIDs[hashValue] = 1;

#ifdef __IGOR_WINDOWS__
        console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (console_handle)
        {
            GetConsoleScreenBufferInfo(console_handle, &console_info);
            _useColorsSupported = true;
        }
#endif

#ifdef __IGOR_LINUX__
        if (isatty(0) &&
            isatty(1) &&
            isatty(2))
        {
            // TODO add some code to actually figure out if it is supported
            _useColorsSupported = true;
        }
#endif
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

    \todo error handling
    */
    void iaConsole::openLogfile()
    {
        if (!_file.is_open())
        {
#ifdef __IGOR_WINDOWS__
            wchar_t result[MAX_PATH];
            GetModuleFileName(NULL, result, MAX_PATH);
            std::wstring path = result;
            path = path.substr(0, path.find_last_of(__IGOR_PATHSEPARATOR__) + 1);
            path.append(L"\\igor.log");
            _file.open(path, std::fstream::out);
#endif

#ifdef __IGOR_LINUX__
            _file.open("/tmp/igor.log", std::fstream::out);
#endif
        }
    }

    void iaConsole::exit()
    {
        printTombstone();

        *this << endlTab << iaForegroundColor::DarkRed << "Sorry Igor could not recover from that. Please feel free to send" << endlTab;
        *this << iaForegroundColor::DarkRed << "me the error log so I can improve Igor for you!" << endlTab;
        *this << iaForegroundColor::White << "Martin Loga (igorgameengine@protonmail.com)" << endl;

#ifdef __IGOR_WINDOWS__
        __debugbreak();
#endif
        std::exit(EXIT_FAILURE);
    }

    void iaConsole::setLogLevel(iaLogLevel logLevel)
    {
        _logLevel = iaLogLevel::Info;
        con_info("log level is " << logLevel);
        _logLevel = logLevel;
    }

    iaConsole& operator<<(iaConsole& console, const iaLogLevel& logLevel)
    {
        const static iaString text[] = {
        "Fatal",
        "Error",
        "Warning",
        "Info",
        "DebugInfo",
        "Debug",
        "Trace" };

        console << text[static_cast<int>(logLevel)].getData();
        return console;
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

    void iaConsole::printCake()
    {
        *this << endlTab;
        *this << iaForegroundColor::Yellow << "    , , ,    " << endlTab;
        *this << iaForegroundColor::White << "    " << iaForegroundColor::Red << "|" << iaForegroundColor::White << "_" << iaForegroundColor::Red << "|" << iaForegroundColor::White << "_" << iaForegroundColor::Red << "|" << iaForegroundColor::White << "   HAPPY" << endlTab;
        *this << iaForegroundColor::White << "   {~" << iaForegroundColor::White << "*" << iaForegroundColor::White << "~" << iaForegroundColor::White << "*" << iaForegroundColor::White << "~} " << endlTab;
        *this << iaForegroundColor::White << "   {~" << iaForegroundColor::Magenta << "*" << iaForegroundColor::White << "~" << iaForegroundColor::Magenta << "*" << iaForegroundColor::White << "~}  BIRTHDAY" << endlTab;
        *this << iaForegroundColor::White << "  {~" << iaForegroundColor::White << "*" << iaForegroundColor::White << "~" << iaForegroundColor::White << "*" << iaForegroundColor::White << "~" << iaForegroundColor::White << "*" << iaForegroundColor::White << "~}" << endlTab;
        *this << iaForegroundColor::Gray << "__" << iaForegroundColor::White << "{~" << iaForegroundColor::Magenta << "*" << iaForegroundColor::White << "~" << iaForegroundColor::Magenta << "*" << iaForegroundColor::White << "~" << iaForegroundColor::Magenta << "*" << iaForegroundColor::White << "~}" << iaForegroundColor::Gray << "__" << iaForegroundColor::White << "   IGOR" << endlTab;
        *this << iaForegroundColor::Gray << "\\___________/" << endlTab << endl;
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

    void iaConsole::printHead(iaLogLevel logLevel)
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
            color = iaForegroundColor::Red;
            tag = " ERROR ";
            break;

        case iaLogLevel::Warning:
            color = iaForegroundColor::Yellow;
            tag = "WARNING";
            break;

        case iaLogLevel::Info:
            color = iaForegroundColor::Cyan;
            tag = " INFO  ";
            break;

        case iaLogLevel::DebugInfo:
        case iaLogLevel::Debug:
            color = iaForegroundColor::White;
            tag = " DEBUG ";
            break;

        case iaLogLevel::Trace:
            color = iaForegroundColor::Magenta;
            tag = " TRACE ";
            break;
        }

        *this << iaForegroundColor::White << "IGOR " << iaForegroundColor::Gray
              << applicationTime << printThreadID << iaForegroundColor::White
              << "[" << color << tag << iaForegroundColor::White << "] ";
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
#ifdef __IGOR_WINDOWS__
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

#ifdef __IGOR_LINUX__
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
        *this << LOCK;
        printHead(iaLogLevel::Info);
        *this << iaForegroundColor::Red << "Errors: " << _errors << iaForegroundColor::Yellow << " Warnings: " << _warnings << endl;
        iaConsole::getInstance() << UNLOCK;
    }

    void iaConsole::activateLogfile(bool activate)
    {
        _streamToLogfile = activate;
    }

}; // namespace iaux