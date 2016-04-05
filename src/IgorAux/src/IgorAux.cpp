// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <IgorAux.h>

#include <iaConsole.h>
#include <iaVersion.h>

#ifdef __IGOR_WIN__
#pragma comment(lib, "DbgHelp.lib")

#include <windows.h>

/*! dll main entry point
gets called for events like loading, unloading
*/
bool WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch(fdwReason)
	{
	case DLL_PROCESS_ATTACH: //!< when loaded
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH: //!< when unloaded
		break;

	default:
		break;
	}

	return true;
}
#endif

#include <ctime>

namespace IgorAux
{

	void printInfo()
	{
#ifdef __IGOR_DEBUG__ 
#define COLOR iaForegroundColor::Gray
#else
#define COLOR iaForegroundColor::DarkGreen
#endif
		iaConsole::getInstance() << LOCK << COLOR << "    IgorAux ver. " << __IGOR_AUX_VERSION__ << " (" << __IGOR_AUX_CONFIGURATION__ << ") LGPL v3.0" << endl << endl << UNLOCK;
#undef COLOR 

        time_t t = time(0);
        struct tm* now = localtime(&t);
        if (now->tm_mon + 1 == 9 &&
            now->tm_mday == 28)
        {
            iaConsole::getInstance().printCake();
        }
	}

	void startup()
	{
	}

	void shutdown()
	{
		iaConsole::getInstance().printStats();
		iaConsole::destroyInstance();
	}

}
