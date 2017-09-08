// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <IgorAux.h>

#include <iaClock.h>
#include <iaVersion.h>

#ifdef __IGOR_WIN__
    #ifdef __IGOR_MSCOMPILER__
        #pragma comment(lib, "DbgHelp.lib")

        #ifdef __IGOR_DEBUG__
            #ifdef __USE_VLD__
                #pragma comment(lib, "vld.lib")
                #include <vld.h>
            #endif
        #endif
    #endif

#include <windows.h>

/*! dll main entry point
gets called for events like loading, unloading
*/
#pragma warning(disable:4100)
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
#pragma warning(default:4100)
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

        // Igor's last reincarnation was 29 September 2012. Before that I was called OpenDC and EasyGL. 
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
		iaClock::initClock();
		iaConsole::getInstance().openLogfile();
	}

	void shutdown()
	{
		iaConsole::getInstance().printStats();
		iaConsole::getInstance().closeLogfile();

		// do not destroy instance iaConsole::destroyInstance();
	}

}
