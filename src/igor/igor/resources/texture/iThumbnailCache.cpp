// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/texture/iThumbnailCache.h>

#include <igor/system/iApplication.h>
#include <igor/threading/iTaskManager.h>
#include <igor/resources/iResourceManager.h>
#include <igor/resources/texture/iTextureFactory.h>

#include <iaux/system/iaDirectory.h>
#include <iaux/system/iaFile.h>

namespace igor
{

    iThumbnailCache &iThumbnailCache::getInstance()
    {
        static iThumbnailCache instance;
        return instance;
    }

    iThumbnailCache::iThumbnailCache()
    {
#ifdef IGOR_LINUX
        _thumbnailCachePath = iaString(std::getenv("HOME")) + "/.cache/Igor/ThumbnailCache";
#endif

#ifdef IGOR_WINDOWS
        // TODO _thumbnailCachePath = "%LOCALAPPDATA%/Igor/ThumbnailCache";
#endif

        if (!iaDirectory::exists(_thumbnailCachePath))
        {
            iaDirectory::makeDirectory(_thumbnailCachePath);
        }

        iWindowPtr window = iApplication::getInstance().getWindow();
        iTaskManager::getInstance().addTask(new iTaskGenerateThumbnails(window));
    }

    iTexturePtr iThumbnailCache::getThumbnail(const iaString &filename)
    {
        iaFile file(filename);
        if(!file.exists())
        {
            return nullptr;
        }        

        iaTime time = file.getLastModifiedTime();

        const iaString hashName = iaString::toString((uint64)filename.getHashValue(), 16);
        const iaString hashTime = iaString::toString((uint64)time.getMicroseconds(), 16);

        const iaString thumbnailFilename = hashName + "-" + hashTime + ".png";
        const iaString thumbnailFilepath = _thumbnailCachePath + "/" + thumbnailFilename;

        if (!iaFile::exists(thumbnailFilepath))
        {
            // look for older files with same hashName and delete them
            iaDirectory dir(_thumbnailCachePath);
            const iaString searchPattern = hashName + "*.png";
            auto files = dir.getFiles(searchPattern);
            for (auto file : files)
            {
                iaFile::remove(file.getFullFileName());
            }

            // put in queue to create new thumbnail later
            _queueMutex.lock();
            _thumbnailProcessQueue.push_back(std::pair<iaString, iaString>(filename, thumbnailFilepath));
            _queueMutex.unlock();
            return nullptr;
        }

        iParameters param({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_TEXTURE},
                           {IGOR_RESOURCE_PARAM_CACHE_MODE, iResourceCacheMode::Cache},
                           {IGOR_RESOURCE_PARAM_SOURCE, thumbnailFilepath},
                           {IGOR_RESOURCE_PARAM_QUIET, true}});

        return iResourceManager::getInstance().requestResource<iTexture>(param);
    }

    void iThumbnailCache::generateThumbnails()
    {
        std::pair<iaString, iaString> info;
        bool skip = false;
        _queueMutex.lock();
        skip = _thumbnailProcessQueue.empty();
        if (!skip)
        {
            info = _thumbnailProcessQueue.front();
            _thumbnailProcessQueue.pop_front();
        }
        _queueMutex.unlock();

        if (skip)
        {
            return;
        }

        iaFile file(info.first);

        iaString extension = file.getExtension();

        for (auto ex : IGOR_SUPPORTED_TEXTURE_EXTENSIONS)
        {
            if (ex == extension)
            {
                iTextureFactory::createThumbnail(info.first, info.second);
                return;
            }
        }

        // TODO handle other formats
    }
}