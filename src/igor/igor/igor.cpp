// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/igor.h>

#include <igor/resources/config/iConfigReader.h>

#include <iaux/iaux.h>
#include <iaux/system/iaDate.h>
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

        iaConsole::getInstance() << T << "    (c) Copyright 2012-2022 by Martin Loga" << endl
                                 << endl;
        iaConsole::getInstance() << T << "    version " << __IGOR_VERSION__ << " (" << __IGOR_CONFIGURATION__ << ") LGPL v3.0" << endl
                                 << endl;
        iaConsole::getInstance() << T << "    thanks to M. Rochel, M. Schulz, T. Drevensek" << endl
                                 << endl;
        iaConsole::getInstance() << T << "    powered by NewtonDynamics, OpenGL, OpenAL-Soft, Glad, stb_image, TinyXML, EnTT" << endl
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

        con_info("current directory \"" << iaDirectory::getCurrentDirectory() << "\"");
    }

    void startup()
    {
        startupArgs(0, nullptr);
    }

    void createModules()
    {
        iTimer::create();
        printInfo();
        iApplication::create();
        iEvaluationManager::create();
        iEntityManager::create();
        iAudio::create();
        // iClipboard::create();
        iActionManager::create();
        iMouse::create();
        iKeyboard::create();
        iPhysics::create();
        iRenderer2::create();
        iMaterialResourceFactory::create();
        iTextureResourceFactory::create();
        iResourceManager::create();
        iWidgetManager::create();
        iSceneFactory::create();
        iNodeManager::create();
        iTaskManager::create();
        iModelResourceFactory::create();
    }

    void destroyModules()
    {
        // don't change the order if you don't know what you are doing
        if (iModelResourceFactory::isInstantiated())
        {
            iModelResourceFactory::destroy();
        }

        if (iTaskManager::isInstantiated())
        {
            iTaskManager::destroy();
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

        if (iResourceManager::isInstantiated())
        {
            iResourceManager::destroy();
        }

        if (iMaterialResourceFactory::isInstantiated())
        {
            iMaterialResourceFactory::destroy();
        }

        if (iTextureResourceFactory::isInstantiated())
        {
            iTextureResourceFactory::destroy();
        }

        if (iRenderer2::isInstantiated())
        {
            iRenderer2::destroy();
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

        /*if (iClipboard::isInstantiated())
        {
            iClipboard::destroy();
        }*/

        if (iAudio::isInstantiated())
        {
            iAudio::destroy();
        }

        if (iEntityManager::isInstantiated())
        {
            iEntityManager::destroy();
        }

        if (iEvaluationManager::isInstantiated())
        {
            iEvaluationManager::destroy();
        }
    }

    void startupArgs(int argc, wchar_t **argv)
    {
        iaux::startup();

        createModules();

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
            const static std::vector<iaString> configLocations = {
                L"config\\igor.xml"};
#endif

#ifdef __IGOR_LINUX__
            const static std::vector<iaString> configLocations = {
                L"~/.Igor/igor.xml",
                L"/etc/igor/igor.xml",
                L"../config/igor.xml"};
#endif

            for (const auto &config : configLocations)
            {
                iaFile file(config);

                if (file.exist())
                {
                    configurationFilepath = file.getFullFileName();
                    break;
                }
            }

        }

        if (!configurationFilepath.isEmpty())
        {
            configReader.readConfiguration(configurationFilepath);
            con_info("load configuration \"" << configurationFilepath << "\"");
        }
        else
        {
            con_crit("found no configuration file");
        }
    }

    void shutdown()
    {
        destroyModules();
        iaux::shutdown();
    }

} // namespace igor
