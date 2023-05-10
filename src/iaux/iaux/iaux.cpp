// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/iaux.h>

#include <iaux/system/iaClock.h>
#include <iaux/system/iaTime.h>
#include <iaux/system/iaConsole.h>
#include <iaux/math/iaRandom.h>

#ifdef __IGOR_WINDOWS__

#include <windows.h>

/*! dll main entry point
gets called for events like loading, unloading
*/
#pragma warning(disable : 4100)
bool WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
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
#pragma warning(default : 4100)
#endif // __IGOR_WINDOWS__

namespace iaux
{

    void startup()
    {
        iaClock::resetClock();
        iaRandom::setSeed(iaTime::getNow().getMicroseconds());

        iaConsole::getInstance().openLogfile();
        con_assert_sticky(1 == sizeof(int8), "sizeof(int8) != 1");
        con_assert_sticky(2 == sizeof(int16), "sizeof(int16) != 2");
        con_assert_sticky(4 == sizeof(int32), "sizeof(int32) != 4");
        con_assert_sticky(8 == sizeof(int64), "sizeof(int64) != 8");
    }

    void shutdown()
    {
        if (iaConsole::getInstance().getErrors() != 0 ||
            iaConsole::getInstance().getWarnings() != 0)
        {
            iaConsole::getInstance().printStats();
        }
        iaConsole::getInstance().closeLogfile();
        iaConsole::getInstance().resetStats();
        // do not destroy instance of iaConsole
    }

} // namespace iaux
