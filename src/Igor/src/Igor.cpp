// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <Igor.h>

#include <IgorAux.h>
#include <iaConsole.h>
#include <iaFile.h>
#include <iaDirectory.h>
#include <iaString.h>
using namespace IgorAux;

#include <iApplication.h>
#include <iKeyboard.h>
#include <iMouse.h>
#include <iModelResourceFactory.h>
#include <iResourceManager.h>
#include <iTextureResourceFactory.h>
#include <iMaterialResourceFactory.h>
#include <iEntityControl.h>
#include <iEntityFactory.h>
#ifdef __USE_OPENAL__
    #include <iSoundResourceFactory.h>
#endif
#include <iTimer.h>
#include <iRenderer.h>
#include <iTaskManager.h>
#include <iConfigReader.h>
#include <iVersion.h>
#include <iNodeFactory.h>
#include <iPhysics.h>

#ifdef __IGOR_MSCOMPILER__
	#pragma comment(lib, "OpenGL32.lib")
	#pragma comment(lib, "glu32.lib")
#ifdef __USE_OPENAL__
	#pragma comment(lib, "alut.lib")
	#pragma comment(lib, "OpenAL32.lib")
#endif
	#pragma comment(lib, "newton.lib")

	#ifdef __IGOR_DEBUG__
		#pragma comment(lib, "IgorAux_d.lib")
        #pragma comment(lib, "OMPF_d.lib")
	#else
		#pragma comment(lib, "IgorAux.lib")
        #pragma comment(lib, "OMPF.lib")
	#endif
#endif

#ifdef __IGOR_WIN__
#include <windows.h>

//! gets called when the dll is loaded into memory
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

namespace Igor
{

    void printInfo()
	{
#ifdef __IGOR_DEBUG__ 
    #define G iaForegroundColor::Gray
    #define W iaForegroundColor::DarkBlue
    #define C1 iaForegroundColor::Gray
    #define C2 iaForegroundColor::Gray
    #define M iaForegroundColor::Gray
    #define T iaForegroundColor::Gray
#else
    #define G iaForegroundColor::Green
    #define W iaForegroundColor::DarkCyan
    #define C1 iaForegroundColor::Green
    #define C2 iaForegroundColor::Green
    #define M iaForegroundColor::Green
    #define T iaForegroundColor::DarkMagenta
#endif

		iaConsole::getInstance() << LOCK << endl;
        iaConsole::getInstance() << G << "   ______                               " << C1 << " |\\___/|" << C2 << "  /\\___/\\            " << endl;
        iaConsole::getInstance() << G << "  /\\__  _\\                              " << C1 << " )     ( " << C2 << " )     (" << endl;
        iaConsole::getInstance() << G << "  \\/_/\\ \\/       __      ___    _ __    " << C1 << "=\\     /=" << C2 << "=\\     /=" << endl;
        iaConsole::getInstance() << G << "     \\ \\ \\     /'_ `\\   / __`\\ /\\`'__\\  " << C1 << "  )   ( " << C2 << "   )   (" << endl;
        iaConsole::getInstance() << G << "      \\_\\ \\__ /\\ \\L\\ \\ /\\ \\L\\ \\\\ \\ \\/   " << C1 << " /     \\  " << C2 << " /   \\" << endl;
        iaConsole::getInstance() << G << "      /\\_____\\\\ \\____ \\\\ \\____/ \\ \\_\\   " << C1 << "|       |" << C2 << " /     \\          " << M << "QQ__" << endl;
        iaConsole::getInstance() << W << "  ____" << G << "\\/_____/" << W << "_" << G << "\\/___L\\ \\\\/___/" << W << "___" << G << "\\/_/" << W << "____" << C1 << "\\__  _/" << W << "__" << C2 << "\\__ __/" << W << "_________" << M << "<\"_(_)" << W << "______" << endl;
        iaConsole::getInstance() << G << "                 /\\____/                " << C1 << "   ( (   " << C2 << "    ))               " << M << "(_" << endl;
        iaConsole::getInstance() << G << "                 \\_/__/  game engine     " << C1 << "   ) )  " << C2 << "   ((" << endl;
        iaConsole::getInstance() << G << "                                        " << C1 << "   (_(    " << C2 << "   \\)" << endl;
		
        iaConsole::getInstance() << T << "    (c) Copyright 2014-2015 by Martin Loga" << endl << endl;
        iaConsole::getInstance() << T << "    ver. " << __IGOR_VERSION__ << " (" << __IGOR_CONFIGURATION__ << ") LGPL v3.0"<< endl << endl;
        iaConsole::getInstance() << T << "    thanks to M. Rochel, M. Schulz, T. Drevensek, I. Yozova" << endl;
		iaConsole::getInstance() << W << "  ____________________________________________________________________________" << endl << endl;
		iaConsole::getInstance() << UNLOCK;

#undef G
#undef W
#undef C1
#undef C2
#undef M
#undef T

        IgorAux::printInfo();
	}

	void startup()
	{
		startup(0, nullptr);
	}

	void startup(int argc, wchar_t** argv)
	{
        IgorAux::startup();
		printInfo();

        iConfigReader configReader;

        if (argv != nullptr)
        {
            if (argc == 3)
            {
                iaString arg1 = argv[1];
                if (arg1 == L"-c")
                {
                    iaString arg1value = argv[2];
                    iaFile file(arg1value);
                    if (file.exist())
                    {
                        configReader.readConfiguration(file.getFullFileName());
                    }
                    else
                    {
                        con_err("config file " << file.getFullFileName() << " does no exist");
                    }
                }
                else
                {
                    con_err("unknown command line parameter");
                }
            }
        }
		else
		{
            iaFile file(L"..\\config\\Igor.xml");
			if(file.exist())
			{
				configReader.readConfiguration(file.getFullFileName());
			}
			else
			{
                iaDirectory::setCurrentDirectory(iaDirectory::getApplicationDirectory());
                iaFile file(L"..\\config\\Igor.xml");
                if (file.exist())
                {
                    configReader.readConfiguration(file.getFullFileName());
                }
                else
                {
                    con_warn("default config file " << file.getFileName() << " not found");
                }
			}
		}
	}

	void shutdown()
	{
		// don't change the shutdown order
        if (iModelResourceFactory::isInstantiated())
        {
            iModelResourceFactory::destroyInstance();
        }

        if(iTaskManager::isInstantiated())
		{
			iTaskManager::destroyInstance();
		}

		if(iTimer::isInstantiated())
		{
			iTimer::destroyInstance();
		}

		if(iApplication::isInstantiated())
		{
			iApplication::destroyInstance();
		}

		if (iNodeFactory::isInstantiated())
		{
			iNodeFactory::destroyInstance();
		}

        if (iResourceManager::isInstantiated())
        {
            iResourceManager::destroyInstance();
        }

        if (iMaterialResourceFactory::isInstantiated())
        {
            iMaterialResourceFactory::destroyInstance();
        }

		if(iTextureResourceFactory::isInstantiated())
		{
			iTextureResourceFactory::destroyInstance();
		}

		if(iRenderer::isInstantiated())
		{
			iRenderer::destroyInstance();
		}

        if(iPhysics::isInstantiated())
        {
            iPhysics::destroyInstance();
        }

        if (iEntityControl::isInstantiated())
        {
            iEntityControl::destroyInstance();
        }

        if (iEntityFactory::isInstantiated())
        {
            iEntityFactory::destroyInstance();
        }

#ifdef __USE_OPENAL__
		if(iSoundResourceFactory::isInstantiated())
		{
			iSoundResourceFactory::destroyInstance();
		}
#endif

		if(iKeyCodeMap::isInstantiated())
		{
			iKeyCodeMap::destroyInstance();
		}

		if(iKeyboard::isInstantiated())
		{
			iKeyboard::destroyInstance();
		}

		if(iMouse::isInstantiated())
		{
			iMouse::destroyInstance();
		}

        IgorAux::shutdown();
	}

}