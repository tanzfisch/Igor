// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/resources/sprite/loader/iSpriteIO.h>

#include <igor/resources/sprite/iSprite.h>
#include <igor/resources/texture/iTexture.h>
#include <igor/resources/iResourceManager.h>
#include <igor/utils/iJson.h>

#include <iaux/system/iaFile.h>

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

    bool iSpriteIO::read(const iaString &filename, iSpritePtr sprite)
    {
        if (!isSprite(filename))
        {
            con_err("not a sprite \"" << filename << "\"");
            return false;
        }

        sprite->_frames.clear();

        json data = iJson::parse(filename);

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
        uint32 pixelPerUnit = iJson::getValue<uint32>(spriteJson, "pixelPerUnit", 1);

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

            bool pixel = iJson::getValue<bool>(frame, "pixel", false);

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

    static void writeFrame(json &spriteJson, const iSprite::iFrame &frame)
    {
        spriteJson["pos"] = frame._rect.getTopLeft();
        spriteJson["size"] = frame._rect.getSize();
        spriteJson["pivot"] = frame._pivot;
        spriteJson["pixel"] = frame._pixel;
    }

    bool iSpriteIO::write(const iaString &filename, iSpritePtr sprite)
    {
        try // for catching json exceptions
        {
            char temp[2048];
            filename.getData(temp, 2048);

            std::ofstream stream;
            stream.open(temp);

            json framesJson = json::array();
            for (const auto &frame : sprite->_frames)
            {
                json spriteJson;
                writeFrame(spriteJson, frame);
                framesJson.push_back(spriteJson);
            }

            json spriteJson;
            if(sprite->getTexture() != nullptr)
            {
                if(sprite->getTexture()->getAlias().isEmpty())
                {
                    spriteJson["texture"] = sprite->getTexture()->getID();
                }
                else
                {
                    spriteJson["texture"] = sprite->getTexture()->getAlias();
                }
            }

            spriteJson["pixelPerUnit"] = (uint32)1; // TODO
            spriteJson["frames"] = framesJson;
            
            json dataJson;
            dataJson["sprite"] = spriteJson;

            stream << dataJson.dump(4);
        }
        catch (const std::exception &e)
        {
            con_err("Caught an exception: " << e.what());
        }
        catch (...)
        {
            con_err("Caught an unknown exception.");
        }
    }

} // namespace igor
