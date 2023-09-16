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

    static bool isTexture(const iaString &filename)
    {
        iaFile file(filename);
        const iaString &fileExtension = file.getExtension();

        for (const auto &extension : IGOR_SUPPORTED_TEXTURE_EXTENSIONS)
        {
            if (fileExtension == extension)
            {
                return true;
            }
        }        

        return false;
    }    

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
        const iaString filepath = iResourceManager::getInstance().getFilePath(resource->getID());
        const iaString filename = iResourceManager::getInstance().resolvePath(filepath);
        iSpritePtr sprite = std::dynamic_pointer_cast<iSprite>(resource);

        if(isTexture(filename))
        {
            sprite->_texture = iResourceManager::getInstance().loadResource<iTexture>(filename);
            return true;
        }

        return loadSprite(filename, sprite);
    }

    void iSpriteFactory::readSpriteElement(TiXmlElement *spriteElement, iSpritePtr sprite)
    {
        TiXmlElement *frame = spriteElement->FirstChildElement("Frame");
        const iaString texture(spriteElement->Attribute("texture"));

        //int32 pixelPerUnit = 1;        
        //spriteElement->Attribute("pixelPerUnit", &pixelPerUnit);

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

        if(sprite->getFrameCount() == 0)
        {
            con_err("no frames defined in \"" << sprite->getAlias() << "\"");
            return false;
        }

        con_debug("loaded sprite \"" << sprite->getAlias() << "\" with " << sprite->getFrameCount() << " frames.");
        return true;
    }

    void iSpriteFactory::unloadResource(iResourcePtr resource)
    {
        // nothing else to do here
    }

    bool iSpriteFactory::matchingType(const iParameters &parameters) const
    {
        if (parameters.getParameter<iaString>("type") == getType())
        {
            return true;
        }

        return isSprite(parameters.getParameter<iaString>("name"));
    }

}; // namespace igor