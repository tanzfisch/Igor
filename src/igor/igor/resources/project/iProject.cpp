// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/project/iProject.h>

#include <igor/utils/iJson.h>

#include <iaux/system/iaDirectory.h>

#include <filesystem>

namespace igor
{

    static const iaString s_defaultTemplate = "igor/projects/default";

    iProject &iProject::getInstance()
    {
        static iProject instance;
        return instance;
    }

    void iProject::load(const iaString &projectFolder)
    {
        con_assert_sticky(iaDirectory::isDirectory(projectFolder), "can't find folder \"" << projectFolder << "\"");

        if (isLoaded())
        {
            unload();
        }

        _projectFolder = projectFolder;
        load();
    }

    void iProject::create(const iaString &projectFolder)
    {
        if (!iaDirectory::exists(projectFolder))
        {
            iaDirectory::makeDirectory(projectFolder);
        }

        if (!iaDirectory::isEmpty(projectFolder))
        {
            con_err("can't create project in folder that is not empty \"" << projectFolder << "\"");
            return;
        }

        iaString templatePath = iResourceManager::getInstance().resolvePath(s_defaultTemplate);
        iaDirectory srcDir(templatePath);

        std::filesystem::path srcPath(srcDir.getFullDirectoryName().getData());
        std::filesystem::path dstPath(projectFolder.getData());
        std::filesystem::copy(srcPath, dstPath, std::filesystem::copy_options::recursive);

        con_info("created project in \"" << projectFolder << "\"");

        load();
    }

    void iProject::load()
    {
        const iaString filenameConfig = _projectFolder + IGOR_PATHSEPARATOR + "project_config.json";
        read(filenameConfig);

        const iaString filenameDictionary = "resource_dictionary.json";
        iResourceManager::getInstance().addSearchPath(_projectFolder);
        iResourceManager::getInstance().loadResourceDictionary(filenameDictionary);

        _isLoaded = true;
        con_info("loaded project \"" << getName() << "\"");
    }

    void iProject::unload()
    {
        iResourceManager::getInstance().removeSearchPath(_projectFolder);
        iResourceManager::getInstance().clearResourceDictionary();

        _projectFolder = "";
        _projectName = "";

        _isLoaded = false;
    }

    void iProject::save()
    {
        const iaString filenameConfig = _projectFolder + IGOR_PATHSEPARATOR + "project_config.json";
        const iaString filenameDictionary = _projectFolder + IGOR_PATHSEPARATOR + "resource_dictionary.json";

        write(filenameConfig);
        iResourceManager::getInstance().saveResourceDictionary(filenameDictionary);
    }

    bool iProject::read(const iaString &filename)
    {
        char temp[2048];
        filename.getData(temp, 2048);

        std::ifstream file(temp);
        json data = json::parse(file);

        _projectName = iJson::getValue<iaString>(data, "projectName", "New Project");

        if(data.contains("scenes"))
        {
            json scenesJson = data["scenes"];

            for(const auto &sceneJson : scenesJson)
            {
                iResourceID sceneID = sceneJson.get<iResourceID>();
            }
        }

        con_debug("loaded project file \"" << filename << "\"");

        return true;
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
        for(auto sceneID : _scenes)
        {
            scenesJson.push_back(sceneID);
        }

        json projectJson = {
            {"projectName", _projectName},
            {"scenes", scenesJson}
        };

        stream << projectJson.dump(4);

        con_debug("written project file " << filename);

        return true;
    }

    const std::vector<iResourceID>& iProject::getScenes() const
    {
        return _scenes;
    }

    const iaString &iProject::getProjectFolder() const
    {
        return _projectFolder;
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

    void iProject::addScene(const iResourceID &sceneID)
    {
        _scenes.push_back(sceneID);
    }

    void iProject::removeScene(const iResourceID &sceneID)
    {
        auto iter = std::find(_scenes.begin(), _scenes.end(), sceneID);
        if(iter != _scenes.end())
        {
            _scenes.erase(iter);
        }
    }

}; // namespace igor
