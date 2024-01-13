// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/igor.h>

#include <igor/resources/config/iConfigReader.h>

#include <iaux/iaux.h>
#include <iaux/system/iaDate.h>
using namespace iaux;

extern const std::vector<iaString> IGOR_SUPPORTED_SPRITE_EXTENSIONS = {"sprite"};
extern const std::vector<iaString> IGOR_SUPPORTED_TEXTURE_EXTENSIONS = {"png", "jpg"};
extern const std::vector<iaString> IGOR_SUPPORTED_SHADER_EXTENSIONS = {"shader"};
extern const std::vector<iaString> IGOR_SUPPORTED_MATERIAL_EXTENSIONS = {"mat"};
extern const std::vector<iaString> IGOR_SUPPORTED_ANIMATION_EXTENSIONS = {"anim"};
extern const std::vector<iaString> IGOR_SUPPORTED_MODEL_EXTENSIONS = {"ompf", "obj"};
extern const std::vector<iaString> IGOR_SUPPORTED_SOUND_EXTENSIONS = {"wav"};

extern const iaString IGOR_RESOURCE_SHADER = "shaderMaterial";
extern const iaString IGOR_RESOURCE_MATERIAL = "material";
extern const iaString IGOR_RESOURCE_ANIMATION = "animation";
extern const iaString IGOR_RESOURCE_TEXTURE = "texture";
extern const iaString IGOR_RESOURCE_SOUND = "sound";
extern const iaString IGOR_RESOURCE_SPRITE = "sprite";
extern const iaString IGOR_RESOURCE_MODEL = "model";

extern const iaString IGOR_RESOURCE_PARAM_ALIAS = "alias";
extern const iaString IGOR_RESOURCE_PARAM_TYPE = "type";
extern const iaString IGOR_RESOURCE_PARAM_TEXTURE_BUILD_MODE = "textureBuildMode";
extern const iaString IGOR_RESOURCE_PARAM_TEXTURE_WRAP_MODE = "wrapMode";
extern const iaString IGOR_RESOURCE_PARAM_ID = "id";
extern const iaString IGOR_RESOURCE_PARAM_CACHE_MODE = "cacheMode";
extern const iaString IGOR_RESOURCE_PARAM_PIXMAP = "pixmap";
extern const iaString IGOR_RESOURCE_PARAM_SOURCE = "source";
extern const iaString IGOR_RESOURCE_PARAM_NODE = "node";
extern const iaString IGOR_RESOURCE_PARAM_EXPORT_MODE = "exportMode";
extern const iaString IGOR_RESOURCE_PARAM_JOIN_VERTICES = "joinVertices";
extern const iaString IGOR_RESOURCE_PARAM_SUB_TYPE = "subType";
extern const iaString IGOR_RESOURCE_PARAM_QUIET = "quiet";
extern const iaString IGOR_RESOURCE_PARAM_SHADER = "shaderMaterial";
extern const iaString IGOR_RESOURCE_PARAM_MATERIAL = "material";
extern const iaString IGOR_RESOURCE_PARAM_PHYSICS_MATERIAL = "physicsMaterial";
extern const iaString IGOR_RESOURCE_PARAM_LOD = "lod";
extern const iaString IGOR_RESOURCE_PARAM_KEEP_MESH = "keepMesh";
extern const iaString IGOR_RESOURCE_PARAM_GENERATE = "generate";
extern const iaString IGOR_RESOURCE_PARAM_SEED = "seed";
extern const iaString IGOR_RESOURCE_PARAM_TEXTURE = "texture";
extern const iaString IGOR_RESOURCE_PARAM_EMISSIVE = "emissive";
extern const iaString IGOR_RESOURCE_PARAM_AMBIENT = "ambient";
extern const iaString IGOR_RESOURCE_PARAM_DIFFUSE = "diffuse";
extern const iaString IGOR_RESOURCE_PARAM_SPECULAR = "specular";
extern const iaString IGOR_RESOURCE_PARAM_SHININESS = "shininess";
extern const iaString IGOR_RESOURCE_PARAM_ALPHA = "alpha";
extern const iaString IGOR_RESOURCE_PARAM_TILING = "tiling";

extern const iaString IGOR_RESOURCE_PARAM_TEXTURE0 = "texture0";
extern const iaString IGOR_RESOURCE_PARAM_TEXTURE1 = "texture1";
extern const iaString IGOR_RESOURCE_PARAM_TEXTURE2 = "texture2";
extern const iaString IGOR_RESOURCE_PARAM_TEXTURE3 = "texture3";

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
#define W iaForegroundColor::Gray
#define C1 iaForegroundColor::Gray
#define C2 iaForegroundColor::Gray
#define M iaForegroundColor::Gray
#define T iaForegroundColor::Gray
#else
#define G iaForegroundColor::Green
#define W iaForegroundColor::Blue
#define C1 iaForegroundColor::Green
#define C2 iaForegroundColor::Green
#define M iaForegroundColor::Green
#define T iaForegroundColor::White
#endif
#define R iaForegroundColor::Red

        // source of text http://patorjk.com/software/taag/#p=display&f=Larry%203D&t=powered%20by%20%0AIgor

        iaConsole::getInstance() << LOCK << endl;
        iaConsole::getInstance() << G << "                                   ______                               " << endl;
        iaConsole::getInstance() << G << "                                 /\\__  _\\                              " << endl;
        iaConsole::getInstance() << G << "                                 \\/_/\\ \\/       __      ___    _ __    " << endl;
        iaConsole::getInstance() << G << "                                    \\ \\ \\     /'_ `\\   / __`\\ /\\`'__\\  " << endl;
        iaConsole::getInstance() << G << "                                     \\_\\ \\__ /\\ \\L\\ \\ /\\ \\L\\ \\\\ \\ \\/   " << endl;
        iaConsole::getInstance() << G << "                                     /\\_____\\\\ \\____ \\\\ \\____/ \\ \\_\\   " << endl;
        iaConsole::getInstance() << W << "  ___________________________________" << G << "\\/_____/" << W << "_" << G << "\\/___L\\ \\\\/___/" << W << "___" << G << "\\/_/" << W << "__________" << endl;
        iaConsole::getInstance() << G << "                                                /\\____/                " << endl;
        iaConsole::getInstance() << T << "      (c) Copyright 2012-2023 by Martin Loga" << G << "   \\_/__/   " << endl;

        iaConsole::getInstance() << endl
                                 << T << "      version " << __IGOR_VERSION__ << " (" << IGOR_BUILD_CONFIGURATION << ") LGPL v3.0" << endl
                                 << endl;
        iaConsole::getInstance() << T << "      powered by NewtonDynamics, OpenGL, OpenAL-Soft, Glad, stb_image" << endl;
        iaConsole::getInstance() << T << "                 EnTT, R.P. Easing, TinyXML and Fun" << endl
                                 << endl;
        iaConsole::getInstance() << T << "      thanks to M. Rochel, M. Schulz, T. Drevensek" << endl
                                 << endl;
        iaConsole::getInstance() << T << "      get sources from https://github.com/tanzfisch/Igor.git" << endl;
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

        if (iWidgetManager::isInstantiated())
        {
            iWidgetManager::destroy();
        }

        if (iSceneFactory::isInstantiated())
        {
            iSceneFactory::destroy();
        }

        if (iNodeManager::isInstantiated())
        {
            iNodeManager::destroy();
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

    void startup()
    {
        // first things first
        iaux::startup();
        iTimer::create();
        printInfo();

        con_info("current directory is \"" << iaDirectory::getCurrentDirectory() << "\"");

#ifdef IGOR_WINDOWS
        static const std::vector<iaString> configLocations = {
            L"project"};
#endif

#ifdef IGOR_LINUX
        static const std::vector<iaString> configLocations = {
            iaString(std::getenv("HOME")) + "/.config/igor",
            L"/etc/igor",
            L"project"};
#endif

        iaString configurationFilepath;

        for (const auto &location : configLocations)
        {
            iaFile file(location + IGOR_PATHSEPARATOR + "igor.xml");

            if (file.exists())
            {
                configurationFilepath = file.getFullFileName();
                break;
            }
        }

        iConfigReader::create();

        if (configurationFilepath.isEmpty())
        {
            con_warn("can't find config file. Using defaults");

#ifdef IGOR_LINUX
            iaString configFolder = iaString(std::getenv("HOME")) + "/.config/igor";
#endif

#ifdef IGOR_WINDOWS
            // iaString configFolder = "%LOCALAPPDATA%/Igor ...// TODO
#endif

            if (!iaDirectory::exists(configFolder))
            {
                iaDirectory::makeDirectory(configFolder);
            }

            iConfigReader::getInstance().writeConfiguration(configFolder + "/igor.xml");
        }
        else
        {
            iConfigReader::getInstance().readConfiguration(configurationFilepath);
        }

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
