// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <Igor.h>

#include <iApplication.h>
#include <iKeyboard.h>
#include <iMouse.h>
#include <iModelResourceFactory.h>
#include <iResourceManager.h>
#include <iTextureResourceFactory.h>
#include <iMaterialResourceFactory.h>
#include <iWidgetManager.h>
#include <iProfiler.h>
#include <iActionManager.h>
#ifdef __USE_OPENAL__
#include <iSoundResourceFactory.h>
#endif
#include <iTimer.h>
#include <iRenderer.h>
#include <iTaskManager.h>
#include <iConfigReader.h>
#include <iVersion.h>
#include <iNodeManager.h>
#include <iSceneFactory.h>
#include <iPhysics.h>

#include <IgorAux.h>
#include <iaConsole.h>
#include <iaFile.h>
#include <iaDirectory.h>
#include <iaString.h>
using namespace IgorAux;

#include <ctime>

#ifdef __IGOR_WIN__
#include <windows.h>

//! gets called when the dll is loaded into memory
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
#define R iaForegroundColor::Red

		// source of text http://patorjk.com/software/taag/#p=display&f=Larry%203D&t=powered%20by%20%0AIgor

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

		iaConsole::getInstance() << T << "    (c) Copyright 2012-2019 by Martin Loga" << endl << endl;
		iaConsole::getInstance() << T << "    ver. " << __IGOR_VERSION__ << " (" << __IGOR_CONFIGURATION__ << ") LGPL v3.0" << endl << endl;
		iaConsole::getInstance() << T << "    thanks to M. Rochel, M. Schulz, T. Drevensek, I. Yozova" << endl << endl;
		iaConsole::getInstance() << T << "    powered by NewtonDynamics, OpenGL, GLee, stb_image, TinyXML, LogaSoft" << endl << endl;
        iaConsole::getInstance() << T << "    get sources from https://github.com/tanzfisch/Igor.git" << endl;
		iaConsole::getInstance() << W << "  ____________________________________________________________________________" << endl << endl;
		iaConsole::getInstance() << UNLOCK;

#undef G
#undef W
#undef C1
#undef C2
#undef M
#undef T

		// Igor's last reincarnation was 29 September 2012. Before that I was called OpenDC and EasyGL. 
		time_t t = time(nullptr);
        struct tm now;
        localtime_s(&now, &t);
		if (now.tm_mon == 8 &&
			now.tm_mday == 28)
		{
			iaConsole::getInstance().printCake();
		}
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
			if (file.exist())
			{
				configReader.readConfiguration(file.getFullFileName());
			}
			else
			{
				// todo maybe we should do that without changing directory?
				iaDirectory::setCurrentDirectory(iaDirectory::getApplicationDirectory());
				file = iaFile(L"..\\config\\Igor.xml");
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
		//! \todo somehow we have to figure out dependencies and shutdown in correct order!
		// don't change the shutdown order if you don't know what you are doing
		if (iModelResourceFactory::isInstantiated())
		{
			iModelResourceFactory::destroyInstance();
		}

		if (iTaskManager::isInstantiated())
		{
			iTaskManager::destroyInstance();
		}

		if (iNodeManager::isInstantiated())
		{			
			iNodeManager::destroyInstance();
		}

		if (iSceneFactory::isInstantiated())
		{
			iSceneFactory::destroyInstance();
		}

		if (iWidgetManager::isInstantiated())
		{
			iWidgetManager::destroyInstance();
		}

		if (iResourceManager::isInstantiated())
		{
			iResourceManager::destroyInstance();
		}

		if (iMaterialResourceFactory::isInstantiated())
		{
			iMaterialResourceFactory::destroyInstance();
		}

		if (iTextureResourceFactory::isInstantiated())
		{
			iTextureResourceFactory::destroyInstance();
		}

		if (iRenderer::isInstantiated())
		{
			iRenderer::destroyInstance();
		}

		if (iPhysics::isInstantiated())
		{
			iPhysics::destroyInstance();
		}

#ifdef __USE_OPENAL__
		if (iSoundResourceFactory::isInstantiated())
		{
			iSoundResourceFactory::destroyInstance();
		}
#endif

		if (iKeyCodeMap::isInstantiated())
		{
			iKeyCodeMap::destroyInstance();
		}

		if (iKeyboard::isInstantiated())
		{
			iKeyboard::destroyInstance();
		}

		if (iMouse::isInstantiated())
		{
			iMouse::destroyInstance();
		}

		if (iTimer::isInstantiated())
		{
			iTimer::destroyInstance();
		}

		if (iApplication::isInstantiated())
		{
			iApplication::destroyInstance();
		}

		if (iProfiler::isInstantiated())
		{
			iProfiler::destroyInstance();
		}

		if (iActionManager::isInstantiated())
		{
			iActionManager::destroyInstance();
		}

		IgorAux::shutdown();
	}

}
