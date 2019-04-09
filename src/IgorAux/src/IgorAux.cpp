// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <IgorAux.h>

#include <iaClock.h>
#include <iaConsole.h>

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
#endif // __IGOR_WIN__

namespace IgorAux
{

	void startup()
	{
		iaClock::initClock();
		iaConsole::getInstance().openLogfile();
	}

	void shutdown()
	{
		iaConsole::getInstance().printStats();
		iaConsole::getInstance().closeLogfile();
		// do not destroy instance of iaConsole
	}

}
