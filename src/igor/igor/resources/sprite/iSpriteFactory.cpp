// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/sprite/iSpriteFactory.h>

#include <igor/resources/iResourceManager.h>

#include <iaux/system/iaFile.h>
#include <iaux/utils/iaJson.h>
using namespace iaux;

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
        return load(fullFilepath, sprite);
    }

    bool iSpriteFactory::load(const iaString &filename, iSpritePtr sprite)
    {
        if (!isSprite(filename))
        {
            con_err("not a sprite \"" << filename << "\"");
            return false;
        }

        sprite->_frames.clear();

        char temp[2048];
        filename.getData(temp, 2048);

        std::ifstream file(temp);
        json data = json::parse(file);

        if (!data.contains("sprite"))
        {
            con_err("incompatible data");
            return false;
        }
        json spriteJson = data["sprite"];

        if (!spriteJson.contains("texture"))
        {
            con_err("incompatible data");
            return false;
        }
        iaString texture = spriteJson["texture"].get<iaString>();
        sprite->_texture = iResourceManager::getInstance().loadResource<iTexture>(texture);

        // TODO pixelPerUnit
        uint32 pixelPerUnit = iaJson::getValue<uint32>(spriteJson, "pixelPerUnit", 1);

        if (!spriteJson.contains("frames"))
        {
            con_err("incompatible data");
            return false;
        }

        json frames = spriteJson["frames"];
        for (auto frame : frames)
        {
            if (!frame.contains("pos") ||
                !frame.contains("size") ||
                !frame.contains("pivot"))
            {
                continue;
            }

            iaVector2f pos = frame["pos"].get<iaVector2f>();
            iaVector2f size = frame["size"].get<iaVector2f>();
            iaVector2f pivot = frame["pivot"].get<iaVector2f>();

            bool pixel = iaJson::getValue<bool>(frame, "pixel", false);

            sprite->addFrame(pos, size, pivot, pixel);
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