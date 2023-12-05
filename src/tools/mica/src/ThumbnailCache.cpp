// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "ThumbnailCache.h"

iTexturePtr ThumbnailCache::getThumbnail(const iaString& filename)
{
    iaFile file(filename);
    con_assert(file.exists(), "file does not exist \"" << filename << "\"");

    iaTime time = file.getLastModifiedTime();

    const iaString hashName = iaString::toString((uint64)filename.getHashValue(), 16);
    const iaString hashTime = iaString::toString((uint64)time.getMicroseconds(), 16);

    const iaString thumbnailPath = hashName + "-" + hashTime + ".png";

    con_endl(filename << " -> " << thumbnailPath);

// TODOOOOOOOO
    return iTexturePtr();
}