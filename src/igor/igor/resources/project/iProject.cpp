// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/project/iProject.h>

#include <igor/resources/iResourceManager.h>

#include <iaux/system/iaDirectory.h>

#include <tinyxml.h>

#include <filesystem>

namespace igor
{

    static const iaString s_defaultTemplate = "igor/projects/default";

    iProjectPtr iProject::loadProject(const iaString &projectFolder)
    {
        con_assert_sticky(iaDirectory::isDirectory(projectFolder), "can't find folder \"" << projectFolder << "\"");

        bool success = true;

        iProjectPtr project(new iProject(projectFolder));
        project->load();

        con_info("loaded project \"" << project->getName() << "\"");

        return project;
    }

    iProjectPtr iProject::createProject(const iaString &projectFolder)
    {
        if (!iaDirectory::exists(projectFolder))
        {
            iaDirectory::makeDirectory(projectFolder);
        }

        if (!iaDirectory::isEmpty(projectFolder))
        {
            con_err("can't create project in folder that is not empty \"" << projectFolder << "\"");
            return nullptr;
        }

        iaString templatePath = iResourceManager::getInstance().resolvePath(s_defaultTemplate);
        iaDirectory srcDir(templatePath);

        std::filesystem::path srcPath(srcDir.getFullDirectoryName().getData());
        std::filesystem::path dstPath(projectFolder.getData());
        std::filesystem::copy(srcPath, dstPath, std::filesystem::copy_options::recursive);

        con_info("created project in \"" << projectFolder << "\"");

        return loadProject(projectFolder);
    }

    void iProject::saveProject(iProjectPtr project)
    {
        con_assert(project != nullptr, "zero pointer");
        project->save();
    }

    void iProject::unloadProject(iProjectPtr project)
    {
        con_assert(project != nullptr, "zero pointer");
        project->unload();       
    }


    iProject::iProject(const iaString &projectFolder)
        : _projectFolder(projectFolder)
    {
    }

    void iProject::load()
    {
        const iaString filenameConfig = _projectFolder + IGOR_PATHSEPARATOR + "project_config.xml";
        const iaString filenameDictionary = _projectFolder + IGOR_PATHSEPARATOR + "resource_dictionary.xml";

        readConfiguration(filenameConfig);
        iResourceManager::getInstance().addSearchPath(_projectFolder);
        iResourceManager::getInstance().loadResourceDictionary(filenameDictionary);
    }

    void iProject::unload()
    {
        iResourceManager::getInstance().removeSearchPath(_projectFolder);
        iResourceManager::getInstance().clearResourceDictionary();
    }    

    void iProject::save()
    {
        const iaString filenameConfig = _projectFolder + IGOR_PATHSEPARATOR + "project_config.xml";
        const iaString filenameDictionary = _projectFolder + IGOR_PATHSEPARATOR + "resource_dictionary.xml";

        writeConfiguration(filenameConfig);
        iResourceManager::getInstance().saveResourceDictionary(filenameDictionary);
    }

    bool iProject::readConfiguration(const iaString &filename)
    {
        char temp[2048];
        filename.getData(temp, 2048);

        TiXmlDocument document(temp);
        if (!document.LoadFile())
        {
            con_err("can't read \"" << filename << "\". " << document.ErrorDesc());
            return false;
        }

        TiXmlElement *root = document.FirstChildElement("Igor");
        if (root == nullptr)
        {
            con_err("not an igor xml file \"" << temp << "\"");
            return false;
        }

        TiXmlElement *project = root->FirstChildElement("Project");
        if (project == nullptr)
        {
            con_err("invalid file \"" << temp << "\"");
            return false;
        }

        _projectName = (project->Attribute("name"));

        con_debug("loaded project file \"" << filename << "\"");

        return true;
    }

    bool iProject::writeConfiguration(const iaString &filename)
    {
        char temp[2048];
        filename.getData(temp, 2048);

        std::wofstream stream;
        stream.open(temp);

        if (!stream.is_open())
        {
            con_err("can't open to write \"" << temp << "\"");
            return false;
        }

        stream << "<?xml version=\"1.0\"?>\n";
        stream << "<Igor>\n";
        stream << "    <Project name=\"" << _projectName << "\" />\n";
        stream << "</Igor>\n";

        con_debug("written project file " << filename);

        return true;
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
    }

}; // namespace igor
