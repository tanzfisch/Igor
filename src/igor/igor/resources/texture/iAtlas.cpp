// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/texture/iAtlas.h>

#include <igor/resources/texture/iTextureResourceFactory.h>
#include <igor/resources/iResourceManager.h>

#include <tinyxml.h>

namespace igor
{

    iAtlasPtr iAtlas::create(const iTextureOldPtr &texture, const iaString &filename)
    {
        return std::make_shared<iAtlas>(texture, filename);
    }

    iAtlas::iAtlas(const iTextureOldPtr &texture, const iaString &filename)
    {
        _texture = texture;

        if (!filename.isEmpty())
        {
            loadFrames(filename);
        }
    }

    void iAtlas::setTexture(const iTextureOldPtr &texture)
    {
        _texture = texture;
    }

    uint32 iAtlas::addFrame(const iaVector2f &pos, const iaVector2f &size, const iaVector2f &origin, bool pixel)
    {
        return addFrame(iaRectanglef(pos, size), origin, pixel);
    }

    uint32 iAtlas::addFrame(const iaRectanglef &rect, const iaVector2f &origin, bool pixel)
    {
        iFrame frame;

        if (pixel)
        {
            frame._rect.setTopLeft(rect._x / _texture->getWidth(), rect._y / _texture->getHeight());
            frame._rect.setSize(rect._width / _texture->getWidth(), rect._height / _texture->getHeight());
            frame._origin = origin;
        }
        else
        {
            frame._rect = rect;
            frame._origin.set(origin._x * _texture->getWidth(), origin._y * _texture->getHeight());
        }

        _frames.push_back(frame);

        return _frames.size() - 1;
    }

    void iAtlas::readAtlas(TiXmlElement *atlas)
    {
        TiXmlElement *frame = atlas->FirstChildElement("Frame");

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

            addFrame(pos, size, origin, iaString::toBool(attrPixel));

        } while ((frame = frame->NextSiblingElement("Frame")) != nullptr);
    }

    void iAtlas::loadFrames(const iaString &filename)
    {
        con_assert_sticky(!filename.isEmpty(), "empty filename");

        iaString path = iResourceManager::getInstance().getPath(filename);

        char temp[2048];
        path.getData(temp, 2048);

        TiXmlDocument document(temp);
        if (!document.LoadFile())
        {
            con_err("can't read \"" << filename << "\"");
            return;
        }

        TiXmlElement *root = document.FirstChildElement("Igor");
        if (root != nullptr)
        {
            TiXmlElement *atlas = root->FirstChildElement("Atlas");
            if (atlas != nullptr)
            {
                readAtlas(atlas);
            }
        }
    }

    const iTextureOldPtr &iAtlas::getTexture() const
    {
        return _texture;
    }

    uint32 iAtlas::getFrameCount() const
    {
        return static_cast<uint32>(_frames.size());
    }

    const iAtlas::iFrame &iAtlas::getFrame(uint32 index) const
    {
        con_assert(index < _frames.size(), "out of range");

        return _frames[index];
    }
}; // namespace igor
