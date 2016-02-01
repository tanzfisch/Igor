// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iTextureResourceFactory.h>
#include <iRenderer.h>
#include <iResourceManager.h>
#include <iPixmap.h>

#include <iaConsole.h>
#include <IgorAux.h>
using namespace IgorAux;

#include <iSTBImageExt.h>

namespace Igor
{

    iTextureResourceFactory::iTextureResourceFactory()
    {
        genDummyTexture();
    }

    iTextureResourceFactory::~iTextureResourceFactory()
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
        flush();

        // no check if it was actually released
        if (!_textures.empty())
        {
            con_err("possible mem leak. not all textures were released.");

            con_endl("non released textures: ");
            for (auto texture : _textures)
            {
                con_endl(texture.second->getFilename());
            }
        }

        _textures.clear();
    }

    shared_ptr<iTexture> iTextureResourceFactory::getDummyTexture()
    {
        return _dummyTexture;
    }

    void iTextureResourceFactory::genDummyTexture()
    {
        const int width = 256;
        const int height = 256;
        const int Bpp = 4;

        uint8 data[width*height*Bpp];

        bool black = false;

        for (uint32 y = 0; y < height; y++)
        {
            if (y % 16 == 0)
            {
                if (black) black = false;
                else black = true;
            }

            for (uint32 x = 0; x < width; x++)
            {
                if (x % 16 == 0)
                {
                    if (black) black = false;
                    else black = true;
                }

                if (black)
                {
                    data[(y*width + x)*Bpp + 0] = 0;
                    data[(y*width + x)*Bpp + 1] = 0;
                    data[(y*width + x)*Bpp + 2] = 0;
                    data[(y*width + x)*Bpp + 3] = (uint8)(255 * (x / (float)width));
                }
                else
                {
                    data[(y*width + x)*Bpp + 0] = (uint8)(255 * (x / (float)width));
                    data[(y*width + x)*Bpp + 1] = (uint8)(255 * ((128 - x) / (float)width));
                    data[(y*width + x)*Bpp + 2] = (uint8)(255 * (y / (float)height));
                    data[(y*width + x)*Bpp + 3] = 255;
                }
            }
        }

        _dummyTexture = shared_ptr<iTexture>(new iTexture("dummyTexture", iTextureBuildMode::Mipmapped), iTexture::private_deleter());
        _dummyTexture->_width = width;
        _dummyTexture->_height = height;
        _dummyTexture->_valid = true;

        
        if (iRenderer::getInstance().isReady())
        {
            _dummyTexture->_rendererTexture = iRenderer::getInstance().createTexture(width, height, 4, iColorFormat::RGBA, data, iTextureBuildMode::Mipmapped);
            iRenderer::getInstance().setDummyTextureID(_dummyTexture->_rendererTexture->_id);
        }

        int64 hashValue = calcHashValue(_dummyTexture->getFilename(), _dummyTexture->getTextureBuildMode());
        _textures[hashValue] = _dummyTexture;

        con_info("loaded texture", "\"" << _dummyTexture->getFilename() << "\" [" << width << ":" << height << "]");
    }

    int64 iTextureResourceFactory::calcHashValue(const iaString& name, iTextureBuildMode mode)
    {
        con_assert(!name.isEmpty(), "invalid parameter");

        std::hash<wstring> hashFunc;

        iaString combined = name;
        if (mode == iTextureBuildMode::Mipmapped)
        {
            combined += "M";
        }

        wstring keyVal = combined.getData();

        return hashFunc(keyVal);
    }

    shared_ptr<iTexture> iTextureResourceFactory::loadFile(const iaString& filename, iTextureBuildMode mode)
    {
        iaString keyPath = iResourceManager::getInstance().getPath(filename);
        if (keyPath.isEmpty())
        {
            keyPath = filename;
        }

        shared_ptr<iTexture> result;
        int64 hashValue = calcHashValue(filename, mode);

        _mutex.lock();
        auto textureIter = _textures.find(hashValue);
        if(textureIter != _textures.end())
        {
            result = (*textureIter).second;
        }

        if (nullptr == result.get())
        {
            result = shared_ptr<iTexture>(new iTexture(keyPath, mode), iTexture::private_deleter());
            loadTexture(result);
            _textures[hashValue] = result;
        }
        _mutex.unlock();

        return result;
    }

    shared_ptr<iTexture> iTextureResourceFactory::requestFile(const iaString& filename, iTextureBuildMode mode)
    {
        iaString keyPath = iResourceManager::getInstance().getPath(filename);
        if (keyPath.isEmpty())
        {
            keyPath = filename;
        }

        shared_ptr<iTexture> result;
        int64 hashValue = calcHashValue(filename, mode);

        _mutex.lock();
        auto textureIter = _textures.find(hashValue);
        if (textureIter != _textures.end())
        {
            result = (*textureIter).second;
        }

        if (nullptr == result.get())
        {
            result = shared_ptr<iTexture>(new iTexture(keyPath, mode), iTexture::private_deleter());
            _textures[hashValue] = result;
        }
        _mutex.unlock();

        return result;
    }

    void iTextureResourceFactory::interruptFlush()
    {
        _interrupLoading = true;
    }

    void iTextureResourceFactory::flush()
    {
        vector<shared_ptr<iTexture>> texturesToProcess;

        _mutex.lock();
        auto texture = _textures.begin();

        while (texture != _textures.end())
        {
            if ((*texture).second->isValid())
            {
                if ((*texture).second.use_count() == 1)
                {
                    iRenderer::getInstance().destroyTexture((*texture).second->_rendererTexture);
                    con_info("released texture", "\"" << (*texture).second->getFilename() << "\"");
                    texture = _textures.erase(texture);
                    continue;
                }
            }
            else
            {
                texturesToProcess.push_back((*texture).second);
            }

            texture++;
        }

        _mutex.unlock();

        if (!_interrupLoading)
        {
            for (auto texture: texturesToProcess)
            {
                loadTexture(texture);
            }
        }

        _interrupLoading = false;
    }

    void iTextureResourceFactory::loadTexture(shared_ptr<iTexture> texture)
    {
        if (!iRenderer::getInstance().isReady())
        {
            return;
        }

        int width = 0;
        int height = 0;
        int components = 0;

        _mutexImageLibrary.lock();
        unsigned char* textureData = stbi_load(texture->getFilename().getData(), &width, &height, &components, 0);
        _mutexImageLibrary.unlock();

        if (textureData == nullptr)
        {
            texture->_dummy = true;
            texture->_valid = true;
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

            texture->_rendererTexture = iRenderer::getInstance().createTexture(width, height, bpp, colorFormat, textureData, texture->_mode);
            texture->_width = width;
            texture->_height = height;
            texture->_dummy = false;
            texture->_valid = true;
            texture->_colorFormat = colorFormat;
            texture->_bpp = bpp;

            con_info("loaded texture", "\"" << texture->getFilename() << "\" [" << width << ":" << height << "] " << ((texture->getTextureBuildMode() == iTextureBuildMode::Mipmapped) ? "mipmapped" : ""));

            delete[] textureData;
        }
    }

    shared_ptr<iTexture> iTextureResourceFactory::loadFromPixmap(iPixmap* pixmap, const iaString& pixmapname, iTextureBuildMode mode)
    {
        shared_ptr<iTexture> result;
        int64 hashValue = calcHashValue(pixmapname, mode);

        _mutex.lock();
        auto texture = _textures.find(hashValue);
        if(texture != _textures.end())
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

            result = shared_ptr<iTexture>(new iTexture(pixmapname, mode), iTexture::private_deleter());
            result->_rendererTexture = iRenderer::getInstance().createTexture(width, height, bpp, colorformat, data, mode);

            _mutex.lock();
            _textures[hashValue] = result;
            _mutex.unlock();
        }

        return result;
    }

    /*!
    \todo refactoring combine loadFileAsPixmap with loadFile
    */
    iPixmap* iTextureResourceFactory::loadFileAsPixmap(const iaString& filename)
    {
        iaString fullPath = iResourceManager::getInstance().getPath(filename);

        uint32 textureid = 0;
        iPixmap *pixmap = nullptr;

		int width = 0;
		int height = 0;
		int components = 0;

		_mutexImageLibrary.lock();
		unsigned char* textureData = stbi_load(fullPath.getData(), &width, &height, &components, 0);
		_mutexImageLibrary.unlock();

        if (textureData == nullptr)
        {
			con_err("can't load \"" << fullPath << "\"");
        }
        else
        {
            pixmap = new iPixmap();
            pixmap->textureID = textureid;

            pixmap->image_width = width;
            pixmap->image_height = height;
            pixmap->bytes_per_pixel = 0;
            pixmap->format = 0;

            //! \todo muss an andere stelle
            switch (components)
            {
            case 3: 
				pixmap->color_format = iColorFormat::RGB; 
				pixmap->bytes_per_pixel = 3;
				break;

            case 4: 
				pixmap->color_format = iColorFormat::RGBA;
				pixmap->bytes_per_pixel = 4;
				break;

            default:
                pixmap->color_format = iColorFormat::Undefined;
                con_warn("unsupported color format");
            };

            pixmap->data = textureData;
        }

        return pixmap;
    }

};
