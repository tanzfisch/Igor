// Igor game engine
// (c) Copyright 2012-2018 by Martin Loga
// see copyright notice in corresponding header file

#include <iResourceManager.h>
#include <iTextureResourceFactory.h>
#include <iModelResourceFactory.h>
#include <iTaskManager.h>

#include <iaFile.h>
#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    void iResourceManager::addSearchPath(const iaString& folder)
    {
        _mutex.lock();

        bool found = false;
        auto iter = _searchPaths.begin();
        while (iter != _searchPaths.end())
        {
            if ((*iter) == folder)
            {
                con_warn("search path " << folder << " already in list");
                found = true;
                break;
            }

            iter++;
        }

        if (!found)
        {
            _searchPaths.push_back(folder);
        }

        _mutex.unlock();
    }

    void iResourceManager::removeSearchPath(const iaString& folder)
    {
        _mutex.lock();

        bool found = false;
        auto iter = _searchPaths.begin();
        while (iter != _searchPaths.end())
        {
            if ((*iter) == folder)
            {
                _searchPaths.erase(iter);
                found = true;
                break;
            }

            iter++;
        }

        if (!found)
        {
            con_warn("search path " << folder << " not found");
        }

        _mutex.unlock();
    }

    void iResourceManager::clear()
    {
        _mutex.lock();

        _searchPaths.clear();

        _mutex.unlock();
    }

    iaString iResourceManager::getPath(const iaString& filename)
    {
        iaFile file(filename);

        if (file.exist())
        {
            return file.getFullFileName();
        }

        iaString result;

        _mutex.lock();

        if (_searchPaths.empty())
        {
            con_warn("no search paths defined");
        }

        auto iter = _searchPaths.begin();
        while (iter != _searchPaths.end())
        {
            iaFile composed((*iter) + iaDirectory::getPathSeperator() + filename);
            if (composed.exist())
            {
                result = composed.getFullFileName();
                break;
            }

            iter++;
        }

        _mutex.unlock();

        return result;
    }

    iaString iResourceManager::getRelativePath(const iaString& filename)
    {
        iaString result = filename;
        for (auto path : _searchPaths)
        {
            iaDirectory dir(path);
            if (filename.findFirstOf(dir.getFullDirectoryName().getData()) != iaString::INVALID_POSITION)
            {
                result = iaDirectory::getRelativePath(dir.getFullDirectoryName().getData(), filename);
                break;
            }
        }

        return result;
    }

}