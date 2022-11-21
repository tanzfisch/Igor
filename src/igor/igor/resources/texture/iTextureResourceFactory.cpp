// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/texture/iTextureResourceFactory.h>
#include <igor/renderer/iRenderer.h>
#include <igor/resources/iResourceManager.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#ifdef __IGOR_WINDOWS__
#pragma warning(disable : 4312)
#endif

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#ifdef __IGOR_WINDOWS__
#pragma warning(default : 4100)
#endif

namespace igor
{

    iTextureResourceFactory::iTextureResourceFactory()
    {
        // TODO need dependencies for modules!
        iRenderer::getInstance().registerInitializedDelegate(iRendererInitializedDelegate(this, &iTextureResourceFactory::init));
        iRenderer::getInstance().registerPreDeinitializeDelegate(iRendererPreDeinitializeDelegate(this, &iTextureResourceFactory::deinit));

        if (iRenderer::getInstance().isReady())
        {
            init();
        }
    }

    iTextureResourceFactory::~iTextureResourceFactory()
    {
        iRenderer::getInstance().unregisterInitializedDelegate(iRendererInitializedDelegate(this, &iTextureResourceFactory::init));
        iRenderer::getInstance().unregisterPreDeinitializeDelegate(iRendererPreDeinitializeDelegate(this, &iTextureResourceFactory::deinit));

        // run a flush once more but only if renderer still exists
        flush(iResourceCacheMode::Keep);

        // now check if it was actually released
        if (!_textures.empty())
        {
            con_err("possible mem leak. not all textures were released.");

            con_endl("non released textures: ");
            for (auto texture : _textures)
            {
                con_endl(texture.second->getFilename() << " ref:" << texture.second.use_count());
            }
        }

        _textures.clear();
    }

    void iTextureResourceFactory::deinit()
    {
        // release dummy texture
        _dummyTexture = nullptr;

        // first remove all textures that where not loaded until now
        auto IterTex = _textures.begin();
        while (IterTex != _textures.end())
        {
            if (!(*IterTex).second->isValid())
            {
                if ((*IterTex).second.use_count() == 1)
                {
                    IterTex = _textures.erase(IterTex);
                    continue;
                }
            }

            IterTex++;
        }

        // than run a flush to clear all released data
        flush(iResourceCacheMode::Keep);
    }

    iTexturePtr iTextureResourceFactory::getDummyTexture()
    {
        return _dummyTexture;
    }

    void iTextureResourceFactory::init()
    {
        const int width = 256;
        const int height = 256;
        const int Bpp = 4;

        uint8 data[width * height * Bpp];

        bool black = false;

        for (uint32 y = 0; y < height; y++)
        {
            if (y % 16 == 0)
            {
                if (black)
                    black = false;
                else
                    black = true;
            }

            for (uint32 x = 0; x < width; x++)
            {
                if (x % 16 == 0)
                {
                    if (black)
                        black = false;
                    else
                        black = true;
                }

                if (black)
                {
                    data[(y * width + x) * Bpp + 0] = 0;
                    data[(y * width + x) * Bpp + 1] = 0;
                    data[(y * width + x) * Bpp + 2] = 0;
                    data[(y * width + x) * Bpp + 3] = (uint8)(255 * (x / (float)width));
                }
                else
                {
                    data[(y * width + x) * Bpp + 0] = (uint8)(255 * (x / (float)width));
                    data[(y * width + x) * Bpp + 1] = (uint8)(255 * ((128 - x) / (float)width));
                    data[(y * width + x) * Bpp + 2] = (uint8)(255 * (y / (float)height));
                    data[(y * width + x) * Bpp + 3] = 255;
                }
            }
        }

        _dummyTexture = iTexturePtr(new iTexture("dummyTexture", iResourceCacheMode::Keep, iTextureBuildMode::Mipmapped, iTextureWrapMode::Repeat));
        _dummyTexture->_width = width;
        _dummyTexture->_height = height;
        _dummyTexture->_valid = true;
        _dummyTexture->_processed = true;

        _dummyTexture->_textureID = iRenderer::getInstance().createTexture(width, height, 4, iColorFormat::RGBA, data, _dummyTexture->_buildMode, _dummyTexture->_wrapMode);
        iRenderer::getInstance().setDummyTextureID(_dummyTexture->_textureID);

        int64 hashValue = calcHashValue(_dummyTexture->getFilename(), _dummyTexture->_cacheMode, _dummyTexture->_buildMode, _dummyTexture->_wrapMode);
        _textures[hashValue] = _dummyTexture;

        con_info("loaded texture \"" << _dummyTexture->getFilename() << "\" [" << width << ":" << height << "]");
    }

    int64 iTextureResourceFactory::calcHashValue(const iaString &name, iResourceCacheMode cacheMode, iTextureBuildMode buildMode, iTextureWrapMode wrapMode)
    {
        con_assert(!name.isEmpty(), "invalid parameter");

        std::hash<std::wstring> hashFunc;

        iaString combined = name;
        switch (cacheMode)
        {
        case iResourceCacheMode::Free:
            combined += "F";
            break;
        case iResourceCacheMode::Cache:
            combined += "C";
            break;
        case iResourceCacheMode::Keep:
            combined += "K";
            break;
        }

        if (buildMode == iTextureBuildMode::Mipmapped)
        {
            combined += "M";
        }
        else
        {
            combined += "N";
        }

        switch (wrapMode)
        {
        case iTextureWrapMode::Repeat:
            combined += "R";
            break;
        case iTextureWrapMode::Clamp:
            combined += "C";
            break;
        case iTextureWrapMode::MirrorRepeat:
            combined += "M";
            break;
        }

        return hashFunc(combined.getData());
    }

    iTexturePtr iTextureResourceFactory::loadFile(const iaString &filename, iResourceCacheMode cacheMode, iTextureBuildMode buildMode, iTextureWrapMode wrapMode)
    {
        iTexturePtr result;

        con_assert_sticky(!filename.isEmpty(), "empty filename");

        if (!iRenderer::getInstance().isReady())
        {
            con_warn("renderer not ready to load textures yet. queued your request.");
            requestFile(filename, cacheMode, buildMode, wrapMode);
        }

        iaString keyPath = iResourceManager::getInstance().getPath(filename);
        if (keyPath.isEmpty())
        {
            keyPath = filename;
        }

        int64 hashValue = calcHashValue(filename, cacheMode, buildMode, wrapMode);

        _mutex.lock();
        auto textureIter = _textures.find(hashValue);
        if (textureIter != _textures.end())
        {
            result = (*textureIter).second;
        }

        if (nullptr == result.get())
        {
            result = iTexturePtr(new iTexture(keyPath, cacheMode, buildMode, wrapMode));
            loadTexture(result);
            _textures[hashValue] = result;
        }
        _mutex.unlock();

        return result;
    }

    iTexturePtr iTextureResourceFactory::requestFile(const iaString &filename, iResourceCacheMode cacheMode, iTextureBuildMode buildMode, iTextureWrapMode wrapMode)
    {
        iTexturePtr result;

        if (!filename.isEmpty())
        {
            iaString keyPath = iResourceManager::getInstance().getPath(filename);
            if (keyPath.isEmpty())
            {
                keyPath = filename;
            }

            int64 hashValue = calcHashValue(filename, cacheMode, buildMode, wrapMode);

            _mutex.lock();
            auto textureIter = _textures.find(hashValue);
            if (textureIter != _textures.end())
            {
                result = (*textureIter).second;
            }
            _mutex.unlock();

            if (nullptr == result.get())
            {
                result = iTexturePtr(new iTexture(keyPath, cacheMode, buildMode, wrapMode));
                _mutex.lock();
                _textures[hashValue] = result;
                _mutex.unlock();
            }
        }

        return result;
    }

    void iTextureResourceFactory::interruptFlush()
    {
        _interrupLoading = true;
    }

    void iTextureResourceFactory::flush(iResourceCacheMode cacheModeLevel)
    {
        std::vector<iTexturePtr> texturesToProcess;

        _mutex.lock();
        auto texture = _textures.begin();

        while (texture != _textures.end())
        {
            if (texture->second->isProcessed())
            {
                if (texture->second->isValid() &&
                    texture->second.use_count() == 1 &&
                    texture->second->_cacheMode <= cacheModeLevel)
                {
                    iRenderer::getInstance().destroyTexture(texture->second->_textureID);
                    con_info("released texture \"" << (*texture).second->getFilename() << "\"");
                    texture = _textures.erase(texture);
                    continue;
                }
            }
            else
            {
                texturesToProcess.push_back(texture->second);
            }

            texture++;
        }

        _mutex.unlock();

        if (iRenderer::getInstance().isReady())
        {
            for (auto texture : texturesToProcess)
            {
                loadTexture(texture);

                if (_interrupLoading)
                {
                    break;
                }
            }
        }

        _interrupLoading = false;
    }

    void iTextureResourceFactory::loadTexture(iTexturePtr texture)
    {
        int width = 0;
        int height = 0;
        int components = 0;

        char temp[1024];
        texture->getFilename().getData(temp, 1024);

        _mutexImageLibrary.lock();
        unsigned char *textureData = stbi_load(temp, &width, &height, &components, 0);
        _mutexImageLibrary.unlock();

        if (textureData == nullptr)
        {
            texture->_dummy = true;
            texture->_valid = false;
            con_err("can't load \"" << texture->getFilename() << "\"");
        }
        else
        {
            long bpp = 0;
            iColorFormat colorFormat = iColorFormat::Undefined;

            switch (components)
            {
            case 3:
                colorFormat = iColorFormat::RGB;
                bpp = 3;
                break;

            case 4:
                colorFormat = iColorFormat::RGBA;
                bpp = 4;
                break;

            default:
                con_assert(false, "unsupported color format");
            };

            texture->_textureID = iRenderer::getInstance().createTexture(width, height, bpp, colorFormat, textureData, texture->_buildMode, texture->_wrapMode);
            texture->_width = width;
            texture->_height = height;
            texture->_dummy = false;
            texture->_valid = true;
            texture->_colorFormat = colorFormat;
            texture->_bpp = bpp;

            iaString build = ".not mipmapped";
            if (texture->_buildMode == iTextureBuildMode::Mipmapped)
            {
                build = ".mipmapped";
            }

            iaString wrap;
            switch (texture->_wrapMode)
            {
            case iTextureWrapMode::Repeat:
                wrap = ".repeat";
                break;

            case iTextureWrapMode::Clamp:
                wrap = ".clamp";
                break;

            case iTextureWrapMode::MirrorRepeat:
                wrap = ".mirror repeat";
                break;
            }

            con_info("loaded texture \"" << texture->getFilename() << "\" [" << width << ":" << height << "] " << build << " " << wrap);

            _mutexImageLibrary.lock();
            stbi_image_free(textureData);
            _mutexImageLibrary.unlock();
        }

        // mark texture processed so we don't have to touch it again
        texture->_processed = true;
    }

    iTexturePtr iTextureResourceFactory::loadFromPixmap(iPixmap *pixmap, const iaString &pixmapname, iResourceCacheMode cacheMode, iTextureBuildMode buildMode, iTextureWrapMode wrapMode)
    {
        iTexturePtr result;
        int64 hashValue = calcHashValue(pixmapname, cacheMode, buildMode, wrapMode);

        _mutex.lock();
        auto texture = _textures.find(hashValue);
        if (texture != _textures.end())
        {
            result = (*texture).second;
        }
        _mutex.unlock();

        if (0 == result.get())
        {
            long width = pixmap->getWidth();
            long height = pixmap->getHeight();
            long bpp = pixmap->getBytesPerPixel();

            uint8 *data = pixmap->getData();
            iColorFormat colorformat = iColorFormat::Undefined;

            switch (bpp)
            {
            case 3:
                colorformat = iColorFormat::RGB;
                break;

            case 4:
                colorformat = iColorFormat::RGBA;
                break;

            default:
                con_err("unknown color format");
            };

            result = iTexturePtr(new iTexture(pixmapname, cacheMode, buildMode, wrapMode));
            result->_textureID = iRenderer::getInstance().createTexture(width, height, bpp, colorformat, data, buildMode, wrapMode);

            _mutex.lock();
            _textures[hashValue] = result;
            _mutex.unlock();
        }

        return result;
    }

    iPixmapPtr iTextureResourceFactory::loadPixmap(const iaString &filename)
    {
        iaString fullPath = iResourceManager::getInstance().getPath(filename);

        iPixmap *pixmap = nullptr;

        int width = 0;
        int height = 0;
        int components = 0;

        char temp[1024];
        fullPath.getData(temp, 1024);

        _mutexImageLibrary.lock();
        unsigned char *textureData = stbi_load(temp, &width, &height, &components, 0);
        _mutexImageLibrary.unlock();

        if (textureData == nullptr)
        {
            con_err("can't load \"" << fullPath << "\"");
        }
        else
        {
            iColorFormat colorFormat = iColorFormat::Undefined;

            switch (components)
            {
            case 3:
                colorFormat = iColorFormat::RGB;
                break;

            case 4:
                colorFormat = iColorFormat::RGBA;
                break;

            default:
                con_warn("unsupported color format");
            };

            pixmap = new iPixmap(width, height, colorFormat);
            pixmap->setData(textureData);
        }

        _mutexImageLibrary.lock();
        stbi_image_free(textureData);
        _mutexImageLibrary.unlock();

        return pixmap;
    }

}; // namespace igor
