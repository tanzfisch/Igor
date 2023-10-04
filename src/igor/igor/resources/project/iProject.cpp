// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/project/iProject.h>

#include <igor/resources/iResourceManager.h>

#include <iaux/system/iaDirectory.h>

#include <filesystem>

namespace igor
{

    static const iaString s_defaultTemplate = "templates/projects/default";

    iProjectPtr iProject::loadProject(const iaString &projectFolder)
    {
        con_assert_sticky(iaDirectory::isDirectory(projectFolder), "can't find folder " << projectFolder);

        return iProjectPtr(new iProject(projectFolder));
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

        return loadProject(projectFolder)
    }

    iProject::iProject(const iaString &projectFolder)
        : _projectFolder(projectFolder)
    {
    }

    const iaString &iProject::getProjectFolder() const
    {
        return _projectFolder;
    }

    const iaString &iProject::getProjectName() const
    {
        return _projectName;
    }

    void iProject::getProjectName(const iaString &projectName)
    {
        _projectName = projectName;
    }

}; // namespace igor
