// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/resources/project/iProject.h>

#include <igor/events/iEventProject.h>
#include <igor/system/iApplication.h>
#include <igor/utils/iJson.h>
#include <igor/entities/iEntitySystemModule.h>
#include <igor/entities/components/iPrefabComponent.h>

#include <iaux/system/iaDirectory.h>
#include <iaux/system/iaFile.h>

#include <filesystem>

namespace igor
{

    static const iaString s_defaultTemplate = "projects/default";
    static const iaString s_defaultProjectFilename = "project_config.project";
    static const iaString s_resourceDictionary = "resource_dictionary.json";

    void iProject::load(const iaString &path)
    {
        if (isLoaded())
        {
            unload();
        }

        if (iaDirectory::isDirectory(path))
        {
            _projectFolder = path;
            _projectFile = s_defaultProjectFilename;
        }
        else
        {
            iaFile projectFile(path);
            _projectFolder = projectFile.getPath();
            _projectFile = projectFile.getFileName();
        }

        load();
    }

    void iProject::create(const iaString &path)
    {
        if (!iaDirectory::exists(path))
        {
            iaDirectory::makeDirectory(path);
        }

        if (!iaDirectory::isEmpty(path))
        {
            con_err("can't create project in folder that is not empty \"" << path << "\"");
            return;
        }

        _projectFolder = path;
        _projectFile = s_defaultProjectFilename;

        iaString templatePath = iResourceManager::getInstance().resolvePath(s_defaultTemplate);
        iaDirectory srcDir(templatePath);

        std::filesystem::path srcPath(srcDir.getAbsoluteDirectoryName().getData());
        std::filesystem::path dstPath(path.getData());
        std::filesystem::copy(srcPath, dstPath, std::filesystem::copy_options::recursive);

        con_info("created project in \"" << path << "\"");

        load();
    }

    void iProject::load()
    {
        if(_isLoaded)
        {
            return;
        }

        const iaString filenameConfig = _projectFolder + IGOR_PATHSEPARATOR + _projectFile;
        const iaString filenameDictionary = s_resourceDictionary;
        iResourceManager::getInstance().addSearchPath(_projectFolder);
        iResourceManager::getInstance().loadResourceDictionary(filenameDictionary);

        _projectSceneAddedEvent.block();
        read(filenameConfig);
        _projectSceneAddedEvent.unblock();

        _isLoaded = true;
        con_info("loaded project \"" << getName() << "\"");

        iApplication::getInstance().onEvent(iEventPtr(new iEventProjectLoaded(filenameConfig)));
    }

    void iProject::unload()
    {
        if(!_isLoaded)
        {
            return;
        }

        for (const auto &sceneID : _scenes)
        {
            iEntitySystemModule::getInstance().destroyScene(sceneID);
        }
        _scenes.clear();

        if (_projectScene != nullptr)
        {
            iEntitySystemModule::getInstance().destroyScene(_projectScene->getID());
            _projectScene = nullptr;
        }

        iResourceManager::getInstance().removeSearchPath(_projectFolder);
        iResourceManager::getInstance().clearResourceDictionary();

        _projectFolder = "";
        _projectName = "";

        _isLoaded = false;
        iApplication::getInstance().onEvent(iEventPtr(new iEventProjectUnloaded()));
    }

    void iProject::save()
    {
        const iaString filenameConfig = _projectFolder + IGOR_PATHSEPARATOR + _projectFile;
        const iaString filenameDictionary = _projectFolder + IGOR_PATHSEPARATOR + s_resourceDictionary;

        write(filenameConfig);
        iResourceManager::getInstance().saveResourceDictionary(filenameDictionary);
    }

    bool iProject::read(const iaString &filename)
    {
        char temp[2048];
        filename.getData(temp, 2048);

        std::ifstream file(temp);
        json projectJson = json::parse(file);
        if (!projectJson.contains("projectName"))
        {
            con_err("no project name found");
            return false;
        }
        _projectName = projectJson["projectName"].get<iaString>();

        if (!projectJson.contains("projectScene"))
        {
            con_err("no project scene found");
            return false;
        }
        json projectSceneJson = projectJson["projectScene"];

        const iEntitySceneID projectSceneID = iJson::getValue<iaUUID>(projectSceneJson, "id", iaUUID());
        _projectScene = iEntitySystemModule::getInstance().createScene(_projectName, projectSceneID, false);
        iEntitySystemModule::getInstance().activateScene(_projectScene);

        if (projectSceneJson.contains("systems"))
        {
            const auto systems = projectSceneJson["systems"].get<std::vector<iaString>>();
            for (const auto &system : systems)
            {
                _projectScene->addSystem(system);
            }
        }

        if (projectSceneJson.contains("quadtree"))
        {
            json quadtreeJson = projectSceneJson["quadtree"];
            _projectScene->initializeQuadtree(quadtreeJson["area"].get<iaRectangled>(),
                                              quadtreeJson["splitThreshold"].get<uint32>(),
                                              quadtreeJson["maxDepth"].get<uint32>());
        }

        if (projectSceneJson.contains("octree"))
        {
            json octreeJson = projectSceneJson["octree"];
            _projectScene->initializeOctree(octreeJson["volume"].get<iAACubed>(),
                                            octreeJson["splitThreshold"].get<uint32>(),
                                            octreeJson["maxDepth"].get<uint32>());
        }

        if (projectJson.contains("scenes"))
        {
            json scenesJson = projectJson["scenes"];

            for (const auto &sceneJson : scenesJson)
            {
                const auto prefabID = iJson::getValue<iResourceID>(sceneJson, "id", iResourceID::getInvalid());
                const bool active = iJson::getValue<bool>(sceneJson, "active", false);
                const auto name = iJson::getValue<iaString>(sceneJson, "name", "");

                addScene(prefabID, name, active);
            }
        }

        con_debug("loaded project file \"" << filename << "\"");

        return true;
    }

    iEntityScenePtr iProject::getProjectScene() const
    {
        return _projectScene;
    }

    static void writeScenes(const std::vector<iEntityPtr> &entities, json &scenesJson)
    {
        for (auto entity : entities)
        {
            auto prefabComponent = entity->getComponent<iPrefabComponent>();
            if (prefabComponent == nullptr)
            {
                continue;
            }

            auto prefab = prefabComponent->getPrefab();
            if (prefab == nullptr)
            {
                continue;
            }

            iResourceManager::getInstance().saveResource(prefabComponent->getPrefab()->getID());

            json sceneJson =
                {
                    {"id", prefabComponent->getPrefab()->getID()},
                    {"active", entity->isActive()},
                    {"name", entity->getName()}};
            scenesJson.push_back(sceneJson);
        }
    }

    bool iProject::write(const iaString &filename)
    {
        char temp[2048];
        filename.getData(temp, 2048);

        std::ofstream stream;
        stream.open(temp);

        if (!stream.is_open())
        {
            con_err("can't open to write \"" << temp << "\"");
            return false;
        }

        json scenesJson = json::array();
        iEntityPtr root = _projectScene->getRootEntity();
        writeScenes(root->getChildren(), scenesJson);
        writeScenes(root->getInactiveChildren(), scenesJson);

        json systemsJson = json::array();
        for (const auto &system : _projectScene->getSystems())
        {
            systemsJson.push_back(system);
        }

        json projectSceneJson =
            {
                {"name", _projectScene->getName()},
                {"id", _projectScene->getID()},
                {"systems", systemsJson}};

        if (_projectScene->hasQuadtree())
        {
            auto quadtree = _projectScene->getQuadtree();
            projectSceneJson["quadtree"] = {
                {"area", quadtree.getArea()},
                {"splitThreshold", quadtree.getSplitThreshold()},
                {"maxDepth", quadtree.getMaxDepth()}};
        }

        if (_projectScene->hasOctree())
        {
            auto octree = _projectScene->getOctree();
            projectSceneJson["octree"] = {
                {"volume", octree.getVolume()},
                {"splitThreshold", octree.getSplitThreshold()},
                {"maxDepth", octree.getMaxDepth()}};
        }

        json projectJson =
            {
                {"projectName", _projectName},
                {"projectScene", projectSceneJson},
                {"scenes", scenesJson}};

        stream << projectJson.dump(4);

        con_debug("written project file " << filename);

        return true;
    }

    const std::vector<iResourceID> &iProject::getScenes() const
    {
        return _scenes;
    }

    const iaString &iProject::getProjectPath() const
    {
        return _projectFolder;
    }

    const iaString iProject::getScenesPath() const
    {
        return _projectFolder + IGOR_PATHSEPARATOR + "scenes";
    }

    const iaString &iProject::getName() const
    {
        return _projectName;
    }

    void iProject::setName(const iaString &projectName)
    {
        _projectName = projectName;
        _hasChanges = true;
    }

    bool iProject::hasChanges() const
    {
        return _hasChanges;
    }

    bool iProject::isLoaded() const
    {
        return _isLoaded;
    }

    iProjectSceneAddedEvent &iProject::getProjectSceneAddedEvent()
    {
        return _projectSceneAddedEvent;
    }

    iProjectSceneRemovedEvent &iProject::getProjectSceneRemovedEvent()
    {
        return _projectSceneRemovedEvent;
    }

    void iProject::addScene(const iResourceID &sceneID, const iaString &name, bool active)
    {
        auto iter = std::find(_scenes.begin(), _scenes.end(), sceneID);
        if (iter != _scenes.end())
        {
            con_warn("scene \"" << name << "\" [" << sceneID << "] was already added to project");
            return;
        }

        _scenes.push_back(sceneID);
        _projectSceneAddedEvent(sceneID);

        iPrefabPtr prefab = iResourceManager::getInstance().requestResource<iPrefab>(sceneID);
        iEntityPtr entityPrefab = _projectScene->createEntity();
        entityPrefab->setName(name);
        entityPrefab->addComponent(new iPrefabComponent(prefab));
        entityPrefab->setActive(active);
    }

    void iProject::removeScene(const iResourceID &sceneID)
    {
        auto iter = std::find(_scenes.begin(), _scenes.end(), sceneID);
        if (iter == _scenes.end())
        {
            return;
        }

        _scenes.erase(iter);
        _projectSceneRemovedEvent(sceneID);
    }

}; // namespace igor
