// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iaConsole.h>
#include <iaDirectory.h>

#include <iostream>
using namespace std;

#include <DbgHelp.h>
#include <iomanip> 
using namespace std;

namespace IgorAux
{

#ifdef __IGOR_WIN__
    // mapping der winapi farben auf die Igor Konsolen Farben
    WORD winapi_colors[] = { FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
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
        FOREGROUND_GREEN | FOREGROUND_RED };

    HANDLE console_handle;
    CONSOLE_SCREEN_BUFFER_INFO console_info;
#endif

    iaConsole::iaConsole()
    {
#ifdef __IGOR_WIN__
        console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (console_handle)
        {
            GetConsoleScreenBufferInfo(console_handle, &console_info);
        }        
#endif

        initializeLogfile();
    }

    iaConsole::~iaConsole()
    {
        if (_file.is_open())
        {
            _file.close();
        }
    }

    /*! Note: can't use classes iaDirectory or iaFile here because of possible deadlock in logging

    \todo error handling
    */
    void iaConsole::initializeLogfile()
    {
        if (!_file.is_open())
        {
            wchar_t result[MAX_PATH];
            GetModuleFileName(NULL, result, MAX_PATH);

#ifdef __IGOR_WIN__
            char separator = '\\';
#else
#error not implemented
#endif
            wstring path = result;
            path = path.substr(0, path.find_last_of(separator) + 1);
            path.append(L"\\Igor.log");
            _file.open(path, fstream::out);
        }
    }

    void iaConsole::printTombstone()
    {
        *this << endl;
        *this << iaForegroundColor::Gray << __IGOR_TAB__  << "     .-------." << endl;
        *this << iaForegroundColor::Gray << __IGOR_TAB__ << "   .'         `." << endl;
        *this << iaForegroundColor::Gray << __IGOR_TAB__ << "   |  " << iaForegroundColor::White << "R  I  P" << iaForegroundColor::Gray << "  |" << endl;
        *this << iaForegroundColor::Gray << __IGOR_TAB__ << "   |           |" << endl;
        *this << iaForegroundColor::Gray << __IGOR_TAB__ << "   |           |" << endl;
        *this << iaForegroundColor::Gray << __IGOR_TAB__ << "   | " << iaForegroundColor::Blue << "@" << iaForegroundColor::Gray << "         |" << endl;
        *this << iaForegroundColor::Gray << __IGOR_TAB__ << " " << iaForegroundColor::Green << "\\" << iaForegroundColor::Gray << " |" << iaForegroundColor::Green << "//       \\" << iaForegroundColor::Gray << " |" << iaForegroundColor::Green << "/" << endl;
        *this << iaForegroundColor::DarkGreen << __IGOR_TAB__ << "^^^^^^^^^^^^^^^^^^^" << endl;
    }

    void iaConsole::printCake()
    {
        *this << endl;
        *this << iaForegroundColor::Yellow << __IGOR_TAB__ << "    , , ,    " << endl;
        *this << iaForegroundColor::White << __IGOR_TAB__ << "    " << iaForegroundColor::Red << "|" << iaForegroundColor::White << "_" << iaForegroundColor::Red << "|" << iaForegroundColor::White << "_" << iaForegroundColor::Red << "|" << iaForegroundColor::White << "   HAPPY" << endl;
        *this << iaForegroundColor::White << __IGOR_TAB__ << "   {~" << iaForegroundColor::White << "*" << iaForegroundColor::White << "~" << iaForegroundColor::White << "*" << iaForegroundColor::White << "~} " << endl;
        *this << iaForegroundColor::White << __IGOR_TAB__ << "   {~" << iaForegroundColor::Magenta << "*" << iaForegroundColor::White << "~" << iaForegroundColor::Magenta << "*" << iaForegroundColor::White << "~}  BIRTHDAY" << endl;
        *this << iaForegroundColor::White << __IGOR_TAB__ << "  {~" << iaForegroundColor::White << "*" << iaForegroundColor::White << "~" << iaForegroundColor::White << "*" << iaForegroundColor::White << "~" << iaForegroundColor::White << "*" << iaForegroundColor::White << "~}" << endl;
        *this << iaForegroundColor::Gray << __IGOR_TAB__ << "__" << iaForegroundColor::White << "{~" << iaForegroundColor::Magenta << "*" << iaForegroundColor::White << "~" << iaForegroundColor::Magenta << "*" << iaForegroundColor::White << "~" << iaForegroundColor::Magenta << "*" << iaForegroundColor::White << "~}" << iaForegroundColor::Gray << "__" << iaForegroundColor::White << "   IGOR" << endl;
        *this << iaForegroundColor::Gray << __IGOR_TAB__ << "\\___________/" << endl << endl;
    }

    /*! thank you Macmade
    http://stackoverflow.com/questions/5693192/win32-backtrace-from-c-code
    */
    void iaConsole::printCallStack(uint32 maxDepth)
    {
#ifdef __IGOR_WIN__
        void* stack[100];

#ifdef __IGOR_X64__
        const int w = 16;
#else
        const int w = 8;
#endif

        HANDLE process = GetCurrentProcess();
        SymInitialize(process, NULL, TRUE);

        unsigned short  frames = CaptureStackBackTrace(0, 100, stack, NULL);
        SYMBOL_INFO* symbol = (SYMBOL_INFO *)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
        symbol->MaxNameLen = 255;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

        if (_streamToLogfile && _file.is_open())
        {
            _file << std::endl;
        }

        for (unsigned int i = 1; i < frames && i < maxDepth; i++)
        {
            SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);

            *this << __IGOR_TAB__ << symbol->Name;
            cout << std::endl;

            if (_streamToLogfile && _file.is_open())
            {
                _file << std::endl;
            }
        }

        free(symbol);
#endif
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
#ifdef __IGOR_WIN__
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
        iaConsole::getInstance() << LOCK << iaForegroundColor::Red << "Errors: " << _errors << iaForegroundColor::Yellow << " Warnings: " << _warnings << endl;
        if (_streamToLogfile && _file.is_open())
        {
            _file << "Errors: " << _errors << " Warnings: " << _warnings << endl;
        }
        iaConsole::getInstance() << UNLOCK;
    }

    void iaConsole::activateLogfile(bool activate)
    {
        _streamToLogfile = activate;
    }

};