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

    iResourcePtr iTextureFactory::createResource(const iResourceParameters &parameters)
    {
        return iResourcePtr(new iTexture(parameters));
    }

    bool iTextureFactory::loadResource(iResourcePtr resource)
    {
        const iaString filename = iResourceManager::getInstance().getPath(resource->getName());
        iTexturePtr texture = std::dynamic_pointer_cast<iTexture>(resource);

        auto &parameters = resource->getParameters()._parameters;

        auto iterGenerate = parameters.find("generate");
        if (iterGenerate != parameters.end())
        {
            if (std::any_cast<bool>(iterGenerate->second))
            {
                return generateTexture(texture, parameters);
            }
        }

        return loadTexture(filename, texture);
    }

    bool iTextureFactory::generateTexture(iTexturePtr texture, const std::map<iaString, std::any> &parameters)
    {
        iTexturePattern pattern = iTexturePattern::SolidColor;
        iaColor4c primary = iaColor4c::white;
        iaColor4c secondary = iaColor4c::black;
        uint32 width = 1;
        uint32 height = 1;
        const int Bpp = 4;

        auto iter = parameters.find("pattern");
        if (iter != parameters.end())
        {
            pattern = std::any_cast<iTexturePattern>(iter->second);
        }

        iter = parameters.find("primary");
        if (iter != parameters.end())
        {
            iaColor4f temp = std::any_cast<iaColor4f>(iter->second);
            iaConvert::convert(temp, primary);
        }

        iter = parameters.find("secondary");
        if (iter != parameters.end())
        {
            iaColor4f temp = std::any_cast<iaColor4f>(iter->second);
            iaConvert::convert(temp, secondary);
        }

        iter = parameters.find("width");
        if (iter != parameters.end())
        {
            width = std::any_cast<uint32>(iter->second);
        }

        iter = parameters.find("height");
        if (iter != parameters.end())
        {
            height = std::any_cast<uint32>(iter->second);
        }

        uint8 *data = new uint8[width * height * Bpp];

        switch (pattern)
        {
        case iTexturePattern::SolidColor:
        {
            for (int i = 0; i < width * height; ++i)
            {
                memcpy(data + i * Bpp, &primary, Bpp);
            }
        }
        break;

        case iTexturePattern::Checker:
        {
            bool black = false;

            for (uint32 y = 0; y < height; y++)
            {
                if (y % 8 == 0)
                {
                    black = !black;
                }

                for (uint32 x = 0; x < width; x++)
                {
                    if (x % 8 == 0)
                    {
                        black = !black;
                    }

                    if (black)
                    {
                        memcpy(data + (y * width + x) * Bpp, &primary, Bpp);
                    }
                    else
                    {
                        memcpy(data + (y * width + x) * Bpp, &secondary, Bpp);
                    }
                }
            }
        }
        break;
        }

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

    iaString iTextureFactory::getHashData(const iResourceParameters &parameters) const
    {
        iaString hashData;
        auto &data = parameters._parameters;

        auto iterWrapMode = data.find("wrapMode");
        if (iterWrapMode != data.end())
        {
            iTextureWrapMode wrapMode = std::any_cast<iTextureWrapMode>(iterWrapMode->second);

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
        }

        auto iterBuildMode = data.find("buildMode");
        if (iterBuildMode != data.end())
        {
            iTextureBuildMode buildMode = std::any_cast<iTextureBuildMode>(iterBuildMode->second);

            if (buildMode == iTextureBuildMode::Mipmapped)
            {
                hashData += "M";
            }
            else
            {
                hashData += "N";
            }
        }

        return hashData;
    }

    bool iTextureFactory::matchingType(const iResourceParameters &parameters) const
    {
        if (parameters._type == getType())
        {
            return true;
        }

        iaFile file(parameters._name);
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