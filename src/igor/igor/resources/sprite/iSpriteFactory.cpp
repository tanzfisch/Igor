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

    iSpriteFactory::iSpriteFactory()
        : iFactory(IGOR_RESOURCE_SPRITE, IGOR_SUPPORTED_SPRITE_EXTENSIONS)
    {
    }

    static bool isSprite(const iaString &filename)
    {
        iaFile file(filename);
        const iaString &fileExtension = file.getExtension();

        for (const auto &extension : IGOR_SUPPORTED_SPRITE_EXTENSIONS)
        {
            if (fileExtension == extension)
            {
                return true;
            }
        }

        return false;
    }

    iResourcePtr iSpriteFactory::createResource()
    {
        iParameters param({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_SPRITE},
                           {IGOR_RESOURCE_PARAM_ID, iaUUID()}});

        return createResource(param);
    }

    iResourcePtr iSpriteFactory::createResource(const iParameters &parameters)
    {
        return iResourcePtr(new iSprite(parameters));
    }

    bool iSpriteFactory::loadResource(iResourcePtr resource)
    {
        iaString filepath = iResourceManager::getInstance().getFilename(resource->getID());
        if (filepath.isEmpty())
        {
            filepath = resource->getSource();
        }
        
        const iaString fullFilepath = iResourceManager::getInstance().resolvePath(filepath);
        iSpritePtr sprite = std::dynamic_pointer_cast<iSprite>(resource);
        return loadSprite(fullFilepath, sprite);
    }

    void iSpriteFactory::readSpriteElement(TiXmlElement *spriteElement, iSpritePtr sprite)
    {
        TiXmlElement *frame = spriteElement->FirstChildElement("Frame");
        const iaString texture(spriteElement->Attribute("texture"));

        // int32 pixelPerUnit = 1;
        // spriteElement->Attribute("pixelPerUnit", &pixelPerUnit);

        sprite->_texture = iResourceManager::getInstance().loadResource<iTexture>(texture);

        do
        {
            iaString attrPos(frame->Attribute("pos"));
            iaString attrSize(frame->Attribute("size"));
            iaString attrPivot(frame->Attribute("pivot"));
            iaString attrPixel(frame->Attribute("pixel"));

            iaVector2f pos;
            iaVector2f size;
            iaVector2f pivot;

            iaString::toVector<float32>(attrPos, pos);
            iaString::toVector<float32>(attrSize, size);
            iaString::toVector<float32>(attrPivot, pivot);

            sprite->addFrame(pos, size, pivot, iaString::toBool(attrPixel));

        } while ((frame = frame->NextSiblingElement("Frame")) != nullptr);
    }

    bool iSpriteFactory::loadSprite(const iaString &filename, iSpritePtr sprite)
    {
        if (!isSprite(filename))
        {
            con_err("not a sprite \"" << filename << "\"");
            return false;
        }

        sprite->_frames.clear();

        char temp[2048];
        filename.getData(temp, 2048);

        TiXmlDocument document(temp);
        if (!document.LoadFile())
        {
            con_err("can't read \"" << filename << "\". " << document.ErrorDesc());
            return false;
        }

        TiXmlElement *root = document.FirstChildElement("Igor");
        if (root != nullptr)
        {
            TiXmlElement *spriteElement = root->FirstChildElement("Sprite");
            if (spriteElement != nullptr)
            {
                readSpriteElement(spriteElement, sprite);
            }
        }

        if (sprite->getFrameCount() == 0)
        {
            con_err("no frames defined for \"" << sprite->getInfo() << "\"");
            return false;
        }

        con_trace("loaded sprite \"" << sprite->getInfo() << "\" with " << sprite->getFrameCount() << " frames.");
        return true;
    }

    void iSpriteFactory::unloadResource(iResourcePtr resource)
    {
        // nothing else to do here
    }

}; // namespace igor