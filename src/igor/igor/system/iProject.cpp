// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/system/iProject.h>

#include <iaux/system/iaDirectory.h>

namespace igor
{

    iProjectPtr iProject::loadProject(const iaString &projectFolder)
    {
        con_assert_sticky(iaDirectory::isDirectory(projectFolder), "can't find folder " << projectFolder);

        return iProjectPtr(new iProject(projectFolder));
    }

    iProject::iProject(const iaString &projectFolder)
        : _projectFolder(projectFolder)
    {
    }

}; // namespace igor
