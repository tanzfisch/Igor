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
        const iaString filename = iResourceManager::getInstance().getPath(resource->getName());
        iTexturePtr texture = std::dynamic_pointer_cast<iTexture>(resource);

        const auto &parameters = resource->getParameters();

        const bool generate = parameters.getValue<bool>("generate", false);
        if (generate)
        {
            return generateTexture(texture, parameters);
        }

        return loadTexture(filename, texture);
    }

    bool iTextureFactory::generateTexture(iTexturePtr texture, const iParameters &parameters)
    {
        const iTexturePattern pattern = parameters.getValue<iTexturePattern>("pattern", iTexturePattern::SolidColor);
        const iaColor4f primaryf = parameters.getValue<iaColor4f>("primary", iaColor4f::white);
        const iaColor4f secondaryf = parameters.getValue<iaColor4f>("secondary", iaColor4f::black);

        iaColor4c primary;
        iaConvert::convert(primaryf, primary);
        iaColor4c secondary;
        iaConvert::convert(secondaryf, secondary);

        texture->_width = parameters.getValue<int32>("width", 1);
        texture->_height = parameters.getValue<int32>("height", 1);
        texture->_bpp = 4;

        uint8 *data = new uint8[texture->_width * texture->_height * texture->_bpp];

        switch (pattern)
        {
        case iTexturePattern::SolidColor:
        {
            for (int i = 0; i < texture->_width * texture->_height; ++i)
            {
                memcpy(data + i * texture->_bpp, &primary, texture->_bpp);
            }
        }
        break;

        case iTexturePattern::CheckerBoard:
        {
            bool black = false;

            for (uint32 y = 0; y < texture->_height; y++)
            {
                if (y % 16 == 0)
                {
                    black = !black;
                }

                for (uint32 x = 0; x < texture->_width; x++)
                {
                    if (x % 16 == 0)
                    {
                        black = !black;
                    }

                    if (black)
                    {
                        memcpy(data + (y * texture->_width + x) * texture->_bpp, &primary, texture->_bpp);
                    }
                    else
                    {
                        memcpy(data + (y * texture->_width + x) * texture->_bpp, &secondary, texture->_bpp);
                    }
                }
            }
        }
        break;
        }

        texture->setData(texture->_width, texture->_height, texture->_bpp, iColorFormat::RGBA, data, texture->_buildMode, texture->_wrapMode);
        con_info("generated texture \"" << texture->getName() << "\" [" << texture->_width << ":" << texture->_height << "] "); // TODO << texture->_buildMode << " " << texture->_wrapMode);
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
            con_err("can't load \"" << texture->getName() << "\"");
            return false;
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

            texture->setData(width, height, bpp, colorFormat, textureData, texture->_buildMode, texture->_wrapMode);
            texture->_useFallback = false;

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

            con_info("loaded texture \"" << texture->getName() << "\" [" << width << ":" << height << "] " << build << " " << wrap);

            _mutexImageLibrary.lock();
            stbi_image_free(textureData);
            _mutexImageLibrary.unlock();
        }

        return true;
    }

    void iTextureFactory::unloadResource(iResourcePtr resource)
    {
        // nothing else to do here
        con_info("released texture \"" << resource->getName() << "\"");
    }

    iaString iTextureFactory::getHashData(const iParameters &parameters) const
    {
        iaString hashData;

        iTextureWrapMode wrapMode = parameters.getValue<iTextureWrapMode>("wrapMode", iTextureWrapMode::Repeat);
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

        iTextureBuildMode buildMode = parameters.getValue<iTextureBuildMode>("buildMode", iTextureBuildMode::Mipmapped);
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
        if (parameters.getValue<iaString>("type") == getType())
        {
            return true;
        }

        iaFile file(parameters.getValue<iaString>("name"));
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

    iPixmap *iTextureFactory::loadPixmap(const iaString &filename)
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