// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/sprite/iSpriteFactory.h>

#include <igor/resources/iResourceManager.h>

#include <iaux/system/iaFile.h>
using namespace iaux;

#include <tinyxml.h>

namespace igor
{

    const iaString &iSpriteFactory::getType() const
    {
        const static iaString typeName(L"sprite");
        return typeName;
    }

    iResourcePtr iSpriteFactory::createResource(const iParameters &parameters)
    {
        return iResourcePtr(new iSprite(parameters));
    }

    bool iSpriteFactory::loadResource(iResourcePtr resource)
    {
        const iaString filename = iResourceManager::getInstance().getPath(resource->getName());
        iSpritePtr sprite = std::dynamic_pointer_cast<iSprite>(resource);

        return loadSprite(filename, sprite);
    }

    void iSpriteFactory::readAtlas(TiXmlElement *atlas, iSpritePtr sprite)
    {
        TiXmlElement *frame = atlas->FirstChildElement("Frame");
        const iaString textureFileName(atlas->Attribute("texture"));

        sprite->_texture = iResourceManager::getInstance().loadResource<iTexture>(textureFileName);

        do
        {
            iaString attrPos(frame->Attribute("pos"));
            iaString attrSize(frame->Attribute("size"));
            iaString attrOrigin(frame->Attribute("origin"));
            iaString attrPixel(frame->Attribute("pixel"));

            iaVector2f pos;
            iaVector2f size;
            iaVector2f origin;

            iaString::toVector<float32>(attrPos, pos);
            iaString::toVector<float32>(attrSize, size);
            iaString::toVector<float32>(attrOrigin, origin);

            sprite->addFrame(pos, size, origin, iaString::toBool(attrPixel));

        } while ((frame = frame->NextSiblingElement("Frame")) != nullptr);
    }

    bool iSpriteFactory::loadSprite(const iaString &filename, iSpritePtr sprite)
    {
        iaString path = iResourceManager::getInstance().getPath(filename);

        char temp[2048];
        path.getData(temp, 2048);

        TiXmlDocument document(temp);
        if (!document.LoadFile())
        {
            con_err("can't read \"" << filename << "\"");
            return false;
        }

        TiXmlElement *root = document.FirstChildElement("Igor");
        if (root != nullptr)
        {
            TiXmlElement *atlas = root->FirstChildElement("Atlas");
            if (atlas != nullptr)
            {
                readAtlas(atlas, sprite);
            }
        }

        con_info("loaded sprite \"" << sprite->getName() << "\" with " << sprite->getFrameCount() << " frames.");
        return true;
    }

    void iSpriteFactory::unloadResource(iResourcePtr resource)
    {
        // nothing else to do here
        con_info("released sprite \"" << resource->getName() << "\"");
    }

    bool iSpriteFactory::matchingType(const iParameters &parameters) const
    {
        if (parameters.getParameter<iaString>("type") == getType())
        {
            return true;
        }

        iaFile file(parameters.getParameter<iaString>("name"));
        const iaString &fileExtension = file.getExtension();
        static const std::vector<iaString> supportedExtensions = {L"atlas", L"sprite"};

        for (const auto &extension : supportedExtensions)
        {
            if (fileExtension == extension)
            {
                return true;
            }
        }

        return false;
    }

}; // namespace igor