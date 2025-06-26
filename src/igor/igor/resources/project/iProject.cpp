// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/resources/project/iProject.h>

#include <igor/utils/iJson.h>
#include <igor/entities/iEntitySystemModule.h>
#include <igor/entities/components/iPrefabComponent.h>
#include <igor/entities/traversal/iEntityCopyTraverser.h>

#include <iaux/system/iaPath.h>

#include <filesystem>

namespace igor
{

    static const iaString s_defaultTemplate = "projects/default";
    static const iaString s_defaultProjectFilename = "project_config.project";
    static const iaString s_resourceDictionary = "resource_dictionary.json";

    void iProject::load(const iaString &path, iMode mode)
    {
        if (isLoaded())
        {
            unload();
        }

        if (iaPath::isDirectory(path))
        {
            _projectFolder = iaPath::fixPath(path);
            _projectFile = s_defaultProjectFilename;
        }
        else
        {
            iaPath projectFile(iaPath::fixPath(path));
            _projectFolder = projectFile.getParentPath();
            _projectFile = projectFile.getName();
        }

        load();

        _mode = mode;
        if (_mode == iMode::Edit)
        {
            _editScene = _activeScene;
        }
        else
        {
            _runtimeScene = _activeScene;
        }
    }

    iProject::iMode iProject::getMode() const
    {
        return _mode;
    }

    void iProject::setMode(iMode mode)
    {
        if (!isLoaded() ||
            _mode == mode)
        {
            return;
        }

        if (mode == iMode::Edit &&
            _editScene == nullptr)
        {
            con_err("edit mode is only available if it was selected during project load");
            return;
        }        

        _mode = mode;

        // right now this assumes that there will only ever be two states
        if (_mode == iMode::Runtime)
        {            
            if (_editScene != nullptr)
            {
                iEntitySystemModule::getInstance().deactivateScene(_editScene);
            }

            _runtimeScene = iEntitySystemModule::getInstance().createScene("runtime");

            iEntityCopyTraverser copyTraverser(_runtimeScene);

            const auto &children = _editScene->getRootEntity()->getChildren();
            const auto &inactiveChildren = _editScene->getRootEntity()->getInactiveChildren();

            for (const auto &child : children)
            {
                copyTraverser.traverse(child);
            }
            for (const auto &child : inactiveChildren)
            {
                copyTraverser.traverse(child);
            }

            _activeScene = _runtimeScene;
        }
        else
        {
            if (_runtimeScene != nullptr)
            {
                iEntitySystemModule::getInstance().destroyScene(_runtimeScene->getID());
                _runtimeScene = nullptr;
            }

            _activeScene = _editScene;
        }

        iEntitySystemModule::getInstance().activateScene(_activeScene);

        _projectReloadedEvent();
    }

    void iProject::create(const iaString &path)
    {
        if (!iaPath::exists(path))
        {
            iaPath::makeDirectory(path);
        }

        if (!iaPath::isEmpty(path))
        {
            con_err("can't create project in folder that is not empty \"" << path << "\"");
            return;
        }

        _projectFolder = path;
        _projectFile = s_defaultProjectFilename;

        iaString templatePath = iResourceManager::getInstance().resolvePath(s_defaultTemplate);
        iaPath srcDir(templatePath);

        std::filesystem::path srcPath(srcDir.getAbsolutePath().getData());
        std::filesystem::path dstPath(path.getData());
        std::filesystem::copy(srcPath, dstPath, std::filesystem::copy_options::recursive);

        con_info("created project in \"" << path << "\"");

        load();
    }

    void iProject::load()
    {
        if (_isLoaded)
        {
            return;
        }

        const iaString projectFile = getProjectFilepath();
        const iaString filenameDictionary = s_resourceDictionary;
        iResourceManager::getInstance().addSearchPath(_projectFolder);
        iResourceManager::getInstance().loadResourceDictionary(filenameDictionary);

        _sceneAddedEvent.block();
        bool success = read(projectFile);
        _sceneAddedEvent.unblock();

        if (!success)
        {
            con_err("failed to load project from \"" << _projectFolder << "\"");
            return;
        }

        _isLoaded = true;
        con_info("loaded project \"" << getName() << "\"");

        _projectLoadedEvent(projectFile);
    }

    void iProject::unload()
    {
        if (!_isLoaded)
        {
            return;
        }

        for (const auto &sceneID : _scenes)
        {
            iEntitySystemModule::getInstance().destroyScene(sceneID);
        }
        _scenes.clear();

        if (_activeScene != nullptr)
        {
            iEntitySystemModule::getInstance().destroyScene(_activeScene->getID());
            _activeScene = nullptr;
        }

        iResourceManager::getInstance().removeSearchPath(_projectFolder);
        iResourceManager::getInstance().clearResourceDictionary();

        _projectFolder = "";
        _projectName = "";

        _isLoaded = false;
        _projectUnloadedEvent();
    }

    void iProject::save()
    {
        if(_mode != iProject::iMode::Edit)
        {
            return;
        }

        const iaString filenameConfig = _projectFolder + IGOR_PATHSEPARATOR + _projectFile;
        const iaString filenameDictionary = _projectFolder + IGOR_PATHSEPARATOR + s_resourceDictionary;

        write(filenameConfig);
        iResourceManager::getInstance().saveResourceDictionary(filenameDictionary);
    }

    bool iProject::read(const iaString &filename)
    {
        json projectJson = iJson::parse(filename);

        if (!projectJson.contains("projectName"))
        {
            con_err("no project name found");
            return false;
        }
        _projectName = projectJson["projectName"].get<iaString>();

        if (!projectJson.contains("projectScene"))
        {
            _activeScene = iEntitySystemModule::getInstance().createScene("project_scene");
            iEntitySystemModule::getInstance().activateScene(_activeScene);
            return true;
        }

        json projectSceneJson = projectJson["projectScene"];
        const iEntitySceneID projectSceneID = iJson::getValue<iaUUID>(projectSceneJson, "id", iaUUID());

        _activeScene = iEntitySystemModule::getInstance().createScene("project_scene", projectSceneID, false);
        iEntitySystemModule::getInstance().activateScene(_activeScene);

        if (projectSceneJson.contains("systems"))
        {
            const auto systems = projectSceneJson["systems"].get<std::vector<iaString>>();
            for (const auto &system : systems)
            {
                _activeScene->addSystem(system);
            }
        }

        if (projectSceneJson.contains("quadtree"))
        {
            json quadtreeJson = projectSceneJson["quadtree"];
            _activeScene->initializeQuadtree(quadtreeJson["area"].get<iaRectangled>(),
                                             quadtreeJson["splitThreshold"].get<uint32>(),
                                             quadtreeJson["maxDepth"].get<uint32>());
        }

        if (projectSceneJson.contains("octree"))
        {
            json octreeJson = projectSceneJson["octree"];
            _activeScene->initializeOctree(octreeJson["volume"].get<iAACubed>(),
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

    iEntityScenePtr iProject::getRootScene() const
    {
        return _activeScene;
    }

    void iProject::saveScene(iEntityPtr prefabEntity)
    {
        auto prefabComp = prefabEntity->getComponent<iPrefabComponent>();
        auto prefab = prefabComp->getPrefab();
        auto prefabScene = iEntitySystemModule::getInstance().getScene(prefab->getSceneID());
        prefabScene->clear();

        iEntityCopyTraverser traverser(prefabScene->getRootEntity(), true);

        for (const auto entity : prefabEntity->getChildren())
        {
            traverser.traverse(entity);
        }

        for (const auto entity : prefabEntity->getInactiveChildren())
        {
            traverser.traverse(entity);
        }

        iResourceManager::getInstance().saveResource(prefab->getID());
    }

    void iProject::writeScenes(const std::vector<iEntityPtr> &entities, json &scenesJson)
    {
        for (auto entity : entities)
        {
            auto prefabComponent = entity->getComponent<iPrefabComponent>();
            if (prefabComponent == nullptr)
            {
                continue;
            }

            saveScene(entity);

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
        iEntityPtr root = _activeScene->getRootEntity();
        writeScenes(root->getChildren(), scenesJson);
        writeScenes(root->getInactiveChildren(), scenesJson);

        json systemsJson = json::array();
        for (const auto &system : _activeScene->getSystems())
        {
            systemsJson.push_back(system);
        }

        json projectSceneJson =
            {
                {"id", _activeScene->getID()},
                {"systems", systemsJson}};

        if (_activeScene->hasQuadtree())
        {
            auto quadtree = _activeScene->getQuadtree();
            projectSceneJson["quadtree"] = {
                {"area", quadtree.getArea()},
                {"splitThreshold", quadtree.getSplitThreshold()},
                {"maxDepth", quadtree.getMaxDepth()}};
        }

        if (_activeScene->hasOctree())
        {
            auto octree = _activeScene->getOctree();
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

    const iaString iProject::getProjectFilepath() const
    {
        return _projectFolder + IGOR_PATHSEPARATOR + _projectFile;
    }

    const iaString &iProject::getName() const
    {
        return _projectName;
    }

    void iProject::setName(const iaString &projectName)
    {
        _projectName = projectName;
    }

    bool iProject::isLoaded() const
    {
        return _isLoaded;
    }

    iSceneAddedEvent &iProject::getSceneAddedEvent()
    {
        return _sceneAddedEvent;
    }

    iSceneRemovedEvent &iProject::getSceneRemovedEvent()
    {
        return _sceneRemovedEvent;
    }

    iProjectLoadedEvent &iProject::getProjectLoadedEvent()
    {
        return _projectLoadedEvent;
    }

    iProjectReloadedEvent &iProject::getProjectReloadedEvent()
    {
        return _projectReloadedEvent;
    }

    iProjectUnloadedEvent &iProject::getProjectUnloadedEvent()
    {
        return _projectUnloadedEvent;
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
        _sceneAddedEvent(sceneID);

        iPrefabPtr prefab = iResourceManager::getInstance().requestResource<iPrefab>(sceneID);
        iEntityPtr entityPrefab = _activeScene->createEntity();
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
        _sceneRemovedEvent(sceneID);
    }
}; // namespace igor
