// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/igor.h>

#include <igor/resources/config/iConfig.h>

#include <iaux/iaux.h>
#include <iaux/system/iaDate.h>
using namespace iaux;

const std::vector<iaString> IGOR_SUPPORTED_SPRITE_EXTENSIONS = {"sprite", "json"};
const std::vector<iaString> IGOR_SUPPORTED_TEXTURE_EXTENSIONS = {"png", "jpg"};
const std::vector<iaString> IGOR_SUPPORTED_SHADER_EXTENSIONS = {"shader", "xml"};
const std::vector<iaString> IGOR_SUPPORTED_MATERIAL_EXTENSIONS = {"mat", "json"};
const std::vector<iaString> IGOR_SUPPORTED_ANIMATION_EXTENSIONS = {"anim", "json"};
const std::vector<iaString> IGOR_SUPPORTED_MODEL_EXTENSIONS = {"ompf", "obj"};
const std::vector<iaString> IGOR_SUPPORTED_SOUND_EXTENSIONS = {"wav"};
const std::vector<iaString> IGOR_SUPPORTED_PREFAB_EXTENSIONS = {"prefab", "scene", "json"};

const iaString IGOR_RESOURCE_SHADER = "shader";
const iaString IGOR_RESOURCE_MATERIAL = "material";
const iaString IGOR_RESOURCE_ANIMATION = "animation";
const iaString IGOR_RESOURCE_TEXTURE = "texture";
const iaString IGOR_RESOURCE_SOUND = "sound";
const iaString IGOR_RESOURCE_SPRITE = "sprite";
const iaString IGOR_RESOURCE_MODEL = "model";
const iaString IGOR_RESOURCE_PREFAB = "prefab";

const iaString IGOR_RESOURCE_PARAM_ALIAS = "alias";
const iaString IGOR_RESOURCE_PARAM_TYPE = "type";
const iaString IGOR_RESOURCE_PARAM_TEXTURE_BUILD_MODE = "textureBuildMode";
const iaString IGOR_RESOURCE_PARAM_TEXTURE_WRAP_MODE = "wrapMode";
const iaString IGOR_RESOURCE_PARAM_ID = "id";
const iaString IGOR_RESOURCE_PARAM_CACHE_MODE = "cacheMode";
const iaString IGOR_RESOURCE_PARAM_PIXMAP = "pixmap";
const iaString IGOR_RESOURCE_PARAM_SOURCE = "source";
const iaString IGOR_RESOURCE_PARAM_NODE = "node";
const iaString IGOR_RESOURCE_PARAM_EXPORT_MODE = "exportMode";
const iaString IGOR_RESOURCE_PARAM_JOIN_VERTICES = "joinVertices";
const iaString IGOR_RESOURCE_PARAM_SUB_TYPE = "subType";
const iaString IGOR_RESOURCE_PARAM_QUIET = "quiet";
const iaString IGOR_RESOURCE_PARAM_SHADER = "shader";
const iaString IGOR_RESOURCE_PARAM_MATERIAL = "material";
const iaString IGOR_RESOURCE_PARAM_PHYSICS_MATERIAL = "physicsMaterial";
const iaString IGOR_RESOURCE_PARAM_LOD = "lod";
const iaString IGOR_RESOURCE_PARAM_KEEP_MESH = "keepMesh";
const iaString IGOR_RESOURCE_PARAM_GENERATE = "generate";
const iaString IGOR_RESOURCE_PARAM_SEED = "seed";
const iaString IGOR_RESOURCE_PARAM_TEXTURE = "texture";
const iaString IGOR_RESOURCE_PARAM_EMISSIVE = "emissive";
const iaString IGOR_RESOURCE_PARAM_AMBIENT = "ambient";
const iaString IGOR_RESOURCE_PARAM_DIFFUSE = "diffuse";
const iaString IGOR_RESOURCE_PARAM_SPECULAR = "specular";
const iaString IGOR_RESOURCE_PARAM_SHININESS = "shininess";
const iaString IGOR_RESOURCE_PARAM_ALPHA = "alpha";
const iaString IGOR_RESOURCE_PARAM_TILING = "tiling";
const iaString IGOR_RESOURCE_PARAM_ENTITY_SCENE_ID = "entitySceneID";

const iaString IGOR_RESOURCE_PARAM_TEXTURE0 = "texture0";
const iaString IGOR_RESOURCE_PARAM_TEXTURE1 = "texture1";
const iaString IGOR_RESOURCE_PARAM_TEXTURE2 = "texture2";
const iaString IGOR_RESOURCE_PARAM_TEXTURE3 = "texture3";

const iaString IGOR_ITEM_DATA_ID = "id";
const iaString IGOR_ITEM_DATA_NAME = "name";
const iaString IGOR_ITEM_DATA_ICON = "icon";
const iaString IGOR_ITEM_DATA_UUID = "uuid";
const iaString IGOR_ITEM_DATA_ENABLED = "enabled";
const iaString IGOR_ITEM_DATA_SCENE_ID = "sceneID";
const iaString IGOR_ITEM_DATA_ENTITY_ID = "entityID";

#ifdef IGOR_DEBUG
const iaString IGOR_BUILD_CONFIGURATION = "debug";
#else
#ifdef RELWITHDEBINFO
const iaString IGOR_BUILD_CONFIGURATION = "release with debug info";
#else
const iaString IGOR_BUILD_CONFIGURATION = "release";
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

    /*! prints a birthday cake in the log

    \param name the name of the birthday child
     */
    void printCake(const iaString &name)
    {
        iaConsole::getInstance() << endlTab;
        iaConsole::getInstance() << iaForegroundColor::Yellow << "    , , ,    " << endlTab;
        iaConsole::getInstance() << iaForegroundColor::White << "    " << iaForegroundColor::Red << "|" << iaForegroundColor::White << "_" << iaForegroundColor::Red << "|" << iaForegroundColor::White << "_" << iaForegroundColor::Red << "|" << iaForegroundColor::White << "   HAPPY" << endlTab;
        iaConsole::getInstance() << iaForegroundColor::White << "   {~" << iaForegroundColor::White << "*" << iaForegroundColor::White << "~" << iaForegroundColor::White << "*" << iaForegroundColor::White << "~} " << endlTab;
        iaConsole::getInstance() << iaForegroundColor::White << "   {~" << iaForegroundColor::Magenta << "*" << iaForegroundColor::White << "~" << iaForegroundColor::Magenta << "*" << iaForegroundColor::White << "~}    BIRTHDAY" << endlTab;
        iaConsole::getInstance() << iaForegroundColor::White << "  {~" << iaForegroundColor::White << "*" << iaForegroundColor::White << "~" << iaForegroundColor::White << "*" << iaForegroundColor::White << "~" << iaForegroundColor::White << "*" << iaForegroundColor::White << "~}" << endlTab;
        iaConsole::getInstance() << iaForegroundColor::Gray << "__" << iaForegroundColor::White << "{~" << iaForegroundColor::Magenta << "*" << iaForegroundColor::White << "~" << iaForegroundColor::Magenta << "*" << iaForegroundColor::White << "~" << iaForegroundColor::Magenta << "*" << iaForegroundColor::White << "~}" << iaForegroundColor::Gray << "__" << iaForegroundColor::White << "   " << name << endlTab;
        iaConsole::getInstance() << iaForegroundColor::Gray << "\\___________/" << endlTab << endl;
    }

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
        iaConsole::getInstance() << T << "      (c) Copyright 2012-2024 by Martin A. Loga" << G << " \\/___/   " << endl;

        iaConsole::getInstance() << endl
                                 << T << "      version " << __IGOR_VERSION__ << " (" << IGOR_BUILD_CONFIGURATION << ") LGPL v3.0" << endl
                                 << endl;
        iaConsole::getInstance() << T << "      powered by NewtonDynamics, OpenGL, OpenAL-Soft, Glad, stb_image" << endl;
        iaConsole::getInstance() << T << "                 R.P. Easing, TinyXML, nlohmann json and Fun" << endl
                                 << endl;
        iaConsole::getInstance() << T << "      thanks to M. Rochel, M. Schulz, T. Drevensek, M. Dederer" << endl
                                 << endl;
        iaConsole::getInstance() << T << "      special thanks to my wife Gabrielle Loga" << endl
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
        if (date.getMonth() == 8 && date.getDay() == 29)
        {
            printCake("IGOR");
        }
        if (date.getMonth() == 7 && date.getDay() == 9)
        {
            printCake("Gabrielle");
        }
        if (date.getMonth() == 12 && date.getDay() == 25)
        {
            printCake("Jesus");
        }
        if (date.getMonth() == 3 && date.getDay() == 29)
        {
            printCake("Martin");
        }
    }

    void createModules()
    {
        iResourceManager::createInstance();
        iApplication::createInstance();
        iAudio::createInstance();
        iActionManager::createInstance();
        iMouse::createInstance();
        iKeyboard::createInstance();
        iPhysics::createInstance();
        iRenderer::createInstance();
        iWidgetManager::createInstance();
        iSceneFactory::createInstance();
        iNodeManager::createInstance();
        iTaskManager::createInstance();
        iEntitySystemModule::createInstance();
        iProject::createInstance();
        iClipboard::createInstance();
    }

    void destroyModules()
    {
        if (iClipboard::isInstantiated())
        {
            iClipboard::destroyInstance();
        }

        if (iProject::isInstantiated())
        {
            iProject::destroyInstance();
        }

        // don't change the order if you don't know what you are doing
        if (iTaskManager::isInstantiated())
        {
            iTaskManager::destroyInstance();
        }

        if (iResourceManager::isInstantiated())
        {
            iResourceManager::destroyInstance();
        }

        if (iEntitySystemModule::isInstantiated())
        {
            iEntitySystemModule::destroyInstance();
        }

        if (iWidgetManager::isInstantiated())
        {
            iWidgetManager::destroyInstance();
        }

        if (iSceneFactory::isInstantiated())
        {
            iSceneFactory::destroyInstance();
        }

        if (iNodeManager::isInstantiated())
        {
            iNodeManager::destroyInstance();
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

        if (iActionManager::isInstantiated())
        {
            iActionManager::destroyInstance();
        }

        if (iAudio::isInstantiated())
        {
            iAudio::destroyInstance();
        }
    }

    void startup()
    {
        // first things first
        iaux::startup();
        iTimer::createInstance();
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
            iaFile file(location + IGOR_PATHSEPARATOR + "igor.json");

            if (file.exists())
            {
                configurationFilepath = file.getFullFileName();
                break;
            }
        }

        iConfig::createInstance();

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

            iConfig::getInstance().write(configFolder + "/igor.json");
        }
        else
        {
            iConfig::getInstance().read(configurationFilepath);
        }

        if (iConfig::getInstance().hasSetting("logLevel"))
        {
            const iaString level = iConfig::getInstance().getValue("logLevel");

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
