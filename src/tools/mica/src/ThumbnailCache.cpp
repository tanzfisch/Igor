// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "ThumbnailCache.h"

ThumbnailCache &ThumbnailCache::getInstance()
{
    static ThumbnailCache instance;
    return instance;
}

ThumbnailCache::ThumbnailCache()
{
#ifdef IGOR_LINUX
    _thumbnailCachePath = iaString(std::getenv("HOME")) + "/.cache/Mica/ThumbnailCache";
#endif

#ifdef IGOR_WINDOWS
    // TODO _thumbnailCachePath = "%LOCALAPPDATA%/Mica/ThumbnailCache";
#endif

    if (!iaDirectory::exists(_thumbnailCachePath))
    {
        iaDirectory::makeDirectory(_thumbnailCachePath);
    }

    iWindowPtr window = iApplication::getInstance().getWindow();
    iTaskManager::getInstance().addTask(new TaskGenerateThumbnails(window));
}

iTexturePtr ThumbnailCache::getThumbnail(const iaString &filename)
{
    iaFile file(filename);
    con_assert(file.exists(), "file does not exist \"" << filename << "\"");

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

void ThumbnailCache::generateThumbnails()
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

    for(auto ex : IGOR_SUPPORTED_TEXTURE_EXTENSIONS)
    {
        if(ex == extension)
        {
            iTextureFactory::createThumbnail(info.first, info.second);
            return;
        }
    }

    // TODO handle other formats
}