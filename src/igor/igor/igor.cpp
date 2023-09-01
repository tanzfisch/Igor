// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/igor.h>

#include <igor/resources/config/iConfigReader.h>

#include <iaux/iaux.h>
#include <iaux/system/iaDate.h>
using namespace iaux;

extern const std::vector<iaString> IGOR_SUPPORTED_SPRITE_EXTENSIONS = {L"sprite"};
extern const std::vector<iaString> IGOR_SUPPORTED_TEXTURE_EXTENSIONS = {L"png", L"jpg"};

#ifdef IGOR_DEBUG
extern const iaString IGOR_BUILD_CONFIGURATION = "debug";
#else
#ifdef RELWITHDEBINFO
extern const iaString IGOR_BUILD_CONFIGURATION = "release with debug info";
#else
extern const iaString IGOR_BUILD_CONFIGURATION = "release";
#endif
#endif

#ifdef IGOR_WINDOWS
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
#ifdef IGOR_DEBUG
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

        iaConsole::getInstance() << T << "    (c) Copyright 2012-2023 by Martin Loga" << endl
                                 << endl;
        iaConsole::getInstance() << T << "    version " << __IGOR_VERSION__ << " (" << IGOR_BUILD_CONFIGURATION << ") LGPL v3.0" << endl
                                 << endl;
        iaConsole::getInstance() << T << "    thanks to M. Rochel, M. Schulz, T. Drevensek" << endl
                                 << endl;
        iaConsole::getInstance() << T << "    powered by NewtonDynamics, OpenGL, OpenAL-Soft, Glad, stb_image, TinyXML" << endl;
        iaConsole::getInstance() << T << "               EnTT, R. Penner Easing" << endl
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
        iaDate date = iaDate::getToday();
        if (date.getMonth() == 8 &&
            date.getDay() == 29)
        {
            iaConsole::getInstance().printCake();
        }
    }

    void createModules()
    {
        iResourceManager::create();
        iApplication::create();
        iAudio::create();
        iActionManager::create();
        iMouse::create();
        iKeyboard::create();
        iPhysics::create();
        iRenderer::create();
        iMaterialResourceFactory::create();
        iWidgetManager::create();
        iSceneFactory::create();
        iNodeManager::create();
        iTaskManager::create();
        iEntitySystemModule::create();
    }

    void destroyModules()
    {
        // don't change the order if you don't know what you are doing
        if (iEntitySystemModule::isInstantiated())
        {
            iEntitySystemModule::destroy();
        }

        if (iTaskManager::isInstantiated())
        {
            iTaskManager::destroy();
        }

        if (iResourceManager::isInstantiated())
        {
            iResourceManager::destroy();
        }

        if (iNodeManager::isInstantiated())
        {
            iNodeManager::destroy();
        }

        if (iSceneFactory::isInstantiated())
        {
            iSceneFactory::destroy();
        }

        if (iWidgetManager::isInstantiated())
        {
            iWidgetManager::destroy();
        }

        if (iMaterialResourceFactory::isInstantiated())
        {
            iMaterialResourceFactory::destroy();
        }

        if (iRenderer::isInstantiated())
        {
            iRenderer::destroy();
        }

        if (iPhysics::isInstantiated())
        {
            iPhysics::destroy();
        }

        if (iKeyboard::isInstantiated())
        {
            iKeyboard::destroy();
        }

        if (iMouse::isInstantiated())
        {
            iMouse::destroy();
        }

        if (iTimer::isInstantiated())
        {
            iTimer::destroy();
        }

        if (iApplication::isInstantiated())
        {
            iApplication::destroy();
        }

        if (iActionManager::isInstantiated())
        {
            iActionManager::destroy();
        }

        if (iAudio::isInstantiated())
        {
            iAudio::destroy();
        }
    }

    void startup(const iaString &configname)
    {
        // first things first
        iaux::startup();
        iTimer::create();
        printInfo();

        con_info("Current directory is \"" << iaDirectory::getCurrentDirectory() << "\"");

#ifdef IGOR_WINDOWS
        static const std::vector<iaString> configLocations = {
            L"config",
            L"..\\config",
            L"..\\..\\config"};
#endif

#ifdef __IGOR_LINUX__
        static const std::vector<iaString> configLocations = {
            L"~/.igor",
            L"/etc/igor",
            L"config",
            L"../config",
            L"../../config"};
#endif

        iaString configurationFilepath;

        for (const auto &location : configLocations)
        {
            iaFile file(location + __IGOR_PATHSEPARATOR__ + configname + ".xml");

            if (file.exist())
            {
                configurationFilepath = file.getFullFileName();
                break;
            }
        }

        if (configurationFilepath.isEmpty())
        {
            con_crit("can't find config file for \"" << configname << "\"");
        }

        iConfigReader::create();
        iConfigReader::getInstance().readConfiguration(configurationFilepath);

        if (iConfigReader::getInstance().hasSetting("logLevel"))
        {
            const iaString level = iConfigReader::getInstance().getValue("logLevel");

            if (level == "Assert")
            {
                iaConsole::getInstance().setLogLevel(iaLogLevel::Fatal);
            }
            else if (level == "Error")
            {
                iaConsole::getInstance().setLogLevel(iaLogLevel::Error);
            }
            else if (level == "Warning")
            {
                iaConsole::getInstance().setLogLevel(iaLogLevel::Warning);
            }
            else if (level == "Info")
            {
                iaConsole::getInstance().setLogLevel(iaLogLevel::Info);
            }
            else if (level == "User")
            {
                iaConsole::getInstance().setLogLevel(iaLogLevel::User);
            }
            else if (level == "Debug")
            {
                iaConsole::getInstance().setLogLevel(iaLogLevel::Debug);
            }
            else if (level == "Trace")
            {
                iaConsole::getInstance().setLogLevel(iaLogLevel::Trace);
            }
        }

        createModules();
    }

    void shutdown()
    {
        destroyModules();
        iaux::shutdown();
    }

} // namespace igor
