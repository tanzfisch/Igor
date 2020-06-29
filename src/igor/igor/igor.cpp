// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/igor.h>

#include <igor/system/iApplication.h>
#include <igor/system/iKeyboard.h>
#include <igor/system/iMouse.h>
#include <igor/resources/model/iModelResourceFactory.h>
#include <igor/resources/iResourceManager.h>
#include <igor/resources/texture/iTextureResourceFactory.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/ui/iWidgetManager.h>
#include <igor/resources/profiler/iProfiler.h>
#include <igor/ui/actions/iActionManager.h>
#include <igor/system/iTimer.h>
#include <igor/graphics/iRenderer.h>
#include <igor/threading/iTaskManager.h>
#include <igor/resources/config/iConfigReader.h>
#include <igor/iVersion.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/scene/iSceneFactory.h>
#include <igor/physics/iPhysics.h>
#include <igor/system/iClipboard.h>

#include <iaux/iaux.h>
#include <iaux/system/iaConsole.h>
#include <iaux/system/iaFile.h>
#include <iaux/system/iaDirectory.h>
#include <iaux/data/iaString.h>
using namespace iaux;

#ifdef __IGOR_DEBUG__
extern const iaString __IGOR_CONFIGURATION__ = "debug";
#else
#ifdef RELWITHDEBINFO
extern const iaString __IGOR_CONFIGURATION__ = "release with debug info";
#else
extern const iaString __IGOR_CONFIGURATION__ = "release";
#endif
#endif

#ifdef __IGOR_WINDOWS__
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

namespace igor
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

		iaConsole::getInstance() << T << "    (c) Copyright 2012-2020 by Martin Loga" << endl
								 << endl;
		iaConsole::getInstance() << T << "    version " << __IGOR_VERSION__ << " (" << __IGOR_CONFIGURATION__ << ") LGPL v3.0" << endl
								 << endl;
		iaConsole::getInstance() << T << "    thanks to M. Rochel, M. Schulz, T. Drevensek, I. Yozova" << endl
								 << endl;
		iaConsole::getInstance() << T << "    powered by NewtonDynamics, OpenGL, OpenAL-Soft, GLee, stb_image, TinyXML" << endl
								 << endl;
		iaConsole::getInstance() << T << "    get sources from https://github.com/tanzfisch/Igor.git" << endl;
		iaConsole::getInstance() << W << "  ____________________________________________________________________________" << endl
								 << endl;
		iaConsole::getInstance() << UNLOCK;

#undef G
#undef W
#undef C1
#undef C2
#undef M
#undef T

		// Igor's last reincarnation was 29 September 2012.
		// Before that this game engine was called EasyGL (ca 2003) and OpenDC (ca 2005).
		if (iTimer::getInstance().getMonth() == 8 &&
			iTimer::getInstance().getDay() == 28)
		{
			iaConsole::getInstance().printCake();
		}

		con_info("current directory \"" << iaDirectory::getCurrentDirectory() << "\"");
	}

	void startup()
	{
		startup(0, nullptr);
	}

	void startup(int argc, wchar_t **argv)
	{
		iaux::startup();
		printInfo();

		iConfigReader configReader;
		iaString configurationFilepath;

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
						configurationFilepath = file.getFullFileName();
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

		// fall back to default configuration file
		if (configurationFilepath.isEmpty())
		{
#ifdef __IGOR_WINDOWS__
			iaFile file(L"..\\config\\igor.xml");

			if (file.exist())
			{
				configurationFilepath = file.getFullFileName();
			}
#endif

#ifdef __IGOR_LINUX__
			const static iaString configLocations[] = {
				L"/etc/igor/igor.xml",
				L"../config/igor.xml"};

			for (int i = 0; i < 2; ++i)
			{
				iaFile file(configLocations[i]);

				if (file.exist())
				{
					configurationFilepath = file.getFullFileName();
					break;
				}
			}

#endif
		}

		if (!configurationFilepath.isEmpty())
		{
			configReader.readConfiguration(configurationFilepath);
			con_info("load configuration \"" << configurationFilepath << "\"");
		}
		else
		{
			con_err("found no configuration file");
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

		if (iClipboard::isInstantiated())
		{
			iClipboard::destroyInstance();
		}

		iaux::shutdown();
	}

} // namespace igor
