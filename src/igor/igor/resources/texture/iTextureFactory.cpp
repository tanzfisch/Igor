// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/texture/iTextureFactory.h>

#include <igor/resources/iResourceManager.h>

#include <iaux/system/iaFile.h>
#include <iaux/data/iaConvert.h>
using namespace iaux;

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace igor
{

    iaMutex iTextureFactory::_mutexImageLibrary;

    const iaString &iTextureFactory::getType() const
    {
        const static iaString typeName(L"texture");
        return typeName;
    }

    iResourcePtr iTextureFactory::createResource(const iParameters &parameters)
    {
        return iResourcePtr(new iTexture(parameters));
    }

    bool iTextureFactory::loadResource(iResourcePtr resource)
    {
        const iaString filepath = iResourceManager::getInstance().getFilePath(resource->getID());
        const iaString filename = iResourceManager::getInstance().resolvePath(filepath);
        iTexturePtr texture = std::dynamic_pointer_cast<iTexture>(resource);

        const auto &parameters = resource->getParameters();

        const bool generate = parameters.getParameter<bool>("generate", false);
        if (generate)
        {
            return generateTexture(texture, parameters);
        }

        iPixmapPtr pixmap = parameters.getParameter<iPixmapPtr>("pixmap", nullptr);
        if (pixmap != nullptr)
        {
            return pixmapToTexture(pixmap, texture);
        }

        return loadTexture(filename, texture);
    }

    bool iTextureFactory::pixmapToTexture(iPixmapPtr pixmap, iTexturePtr texture)
    {
        uint32 width = pixmap->getWidth();
        uint32 height = pixmap->getHeight();
        uint32 bpp = pixmap->getBytesPerPixel();

        uint8 *data = pixmap->getData();
        iColorFormat colorFormat = iColorFormat::Undefined;

        switch (bpp)
        {
        case 3:
            colorFormat = iColorFormat::RGB;
            break;

        case 4:
            colorFormat = iColorFormat::RGBA;
            break;

        default:
            con_err("unknown color format");
            return false;
        };

        texture->setData(width, height, bpp, colorFormat, data, texture->_buildMode, texture->_wrapMode);

        return true;
    }

    bool iTextureFactory::generateTexture(iTexturePtr texture, const iParameters &parameters)
    {
        const iTexturePattern pattern = parameters.getParameter<iTexturePattern>("pattern", iTexturePattern::SolidColor);
        const iaColor4f primaryf = parameters.getParameter<iaColor4f>("primary", iaColor4f::white);
        const iaColor4f secondaryf = parameters.getParameter<iaColor4f>("secondary", iaColor4f::black);

        iaColor4c primary;
        iaConvert::convert(primaryf, primary);
        iaColor4c secondary;
        iaConvert::convert(secondaryf, secondary);

        const uint32 width = parameters.getParameter<int32>("width", 1);
        const uint32 height = parameters.getParameter<int32>("height", 1);
        const uint32 bpp = 4;

        uint8 *data = new uint8[width * height * bpp];

        switch (pattern)
        {
        case iTexturePattern::SolidColor:
        {
            for (int i = 0; i < width * height; ++i)
            {
                memcpy(data + i * bpp, &primary, bpp);
            }
        }
        break;

        case iTexturePattern::CheckerBoard:
        {
            bool black = false;

            for (uint32 y = 0; y < height; y++)
            {
                if (y % 16 == 0)
                {
                    black = !black;
                }

                for (uint32 x = 0; x < width; x++)
                {
                    if (x % 16 == 0)
                    {
                        black = !black;
                    }

                    if (black)
                    {
                        memcpy(data + (y * width + x) * bpp, &primary, bpp);
                    }
                    else
                    {
                        memcpy(data + (y * width + x) * bpp, &secondary, bpp);
                    }
                }
            }
        }
        break;
        }

        texture->setData(width, height, bpp, iColorFormat::RGBA, data, texture->_buildMode, texture->_wrapMode);
        con_debug("generated texture \"" << texture->getAlias() << "\" [" << texture->_width << ":" << texture->_height << "] build:" << texture->_buildMode << " wrap:" << texture->_wrapMode);
        texture->_useFallback = false;

        delete[] data;

        return true;
    }

    bool iTextureFactory::loadTexture(const iaString &filename, iTexturePtr texture)
    {
        int width = 0;
        int height = 0;
        int components = 0;

        char temp[1024];
        filename.getData(temp, 1024);

        _mutexImageLibrary.lock();
        unsigned char *textureData = stbi_load(temp, &width, &height, &components, 0);
        _mutexImageLibrary.unlock();

        if (textureData == nullptr)
        {
            texture->_useFallback = true;
            _mutexImageLibrary.lock();
            con_err("can't load \"" << texture->getAlias() << "\" reason:" << stbi_failure_reason());
            _mutexImageLibrary.unlock();

            return false;
        }

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

        texture->setData(width, height, bpp, colorFormat, textureData, texture->_buildMode, texture->_wrapMode);
        con_debug("loaded texture \"" << texture->getAlias() << "\" [" << width << ":" << height << "] build:" << texture->_buildMode << " wrap:" << texture->_wrapMode);
        texture->_useFallback = false;

        _mutexImageLibrary.lock();
        stbi_image_free(textureData);
        _mutexImageLibrary.unlock();

        return true;
    }

    void iTextureFactory::unloadResource(iResourcePtr resource)
    {
        // nothing else to do here
    }

    iaString iTextureFactory::getHashData(const iParameters &parameters) const
    {
        iaString hashData;

        iTextureWrapMode wrapMode = parameters.getParameter<iTextureWrapMode>("wrapMode", iTextureWrapMode::Repeat);
        switch (wrapMode)
        {
        case iTextureWrapMode::Repeat:
            hashData += "R";
            break;
        case iTextureWrapMode::Clamp:
            hashData += "C";
            break;
        case iTextureWrapMode::MirrorRepeat:
            hashData += "M";
            break;
        }

        iTextureBuildMode buildMode = parameters.getParameter<iTextureBuildMode>("buildMode", iTextureBuildMode::Mipmapped);
        if (buildMode == iTextureBuildMode::Mipmapped)
        {
            hashData += "M";
        }
        else
        {
            hashData += "N";
        }

        return hashData;
    }

    bool iTextureFactory::matchingType(const iParameters &parameters) const
    {
        if (parameters.getParameter<iaString>("type") == getType())
        {
            return true;
        }

        iaFile file(parameters.getParameter<iaString>("name"));
        const iaString &fileExtension = file.getExtension();
        static const std::vector<iaString> supportedExtensions = {L"png", L"jpg"};

        for (const auto &extension : supportedExtensions)
        {
            if (fileExtension == extension)
            {
                return true;
            }
        }

        return false;
    }

    iPixmapPtr iTextureFactory::loadPixmap(const iaString &filename)
    {
        iaString fullPath = iResourceManager::getInstance().resolvePath(filename);

        iPixmapPtr pixmap;

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
            con_err("can't load \"" << fullPath << "\" reason:" << stbi_failure_reason());
            return pixmap;
        }

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

        pixmap = iPixmap::createPixmap(width, height, colorFormat);
        pixmap->setData(textureData);

        con_debug("loaded texture as pixmap \"" << fullPath << "\" [" << width << ":" << height << "] ");

        _mutexImageLibrary.lock();
        stbi_image_free(textureData);
        _mutexImageLibrary.unlock();

        return pixmap;
    }
}; // namespace igor