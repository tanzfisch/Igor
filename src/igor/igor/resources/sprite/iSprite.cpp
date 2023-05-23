// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/sprite/iSprite.h>

#include <igor/resources/iResourceManager.h>

namespace igor
{

    iSprite::iSprite(const iParameters &parameters)
        : iResource("sprite", parameters)
    {
        addFrame(iaVector2f(), iaVector2f(1.0f, 1.0f), iaVector2f(0.5f, 0.5f), false);
    }

    uint32 iSprite::addFrame(const iaVector2f &pos, const iaVector2f &size, const iaVector2f &origin, bool pixel)
    {
        return addFrame(iaRectanglef(pos, size), origin, pixel);
    }

    uint32 iSprite::addFrame(const iaRectanglef &rect, const iaVector2f &origin, bool pixel)
    {
        iFrame frame;

        if (pixel)
        {
            frame._rect.setTopLeft(rect._x / _texture->getWidth(), rect._y / _texture->getHeight());
            frame._rect.setSize(rect._width / _texture->getWidth(), rect._height / _texture->getHeight());
            frame._origin.set(origin._x / rect._width, origin._y / rect._height);
        }
        else
        {
            frame._rect = rect;
            frame._origin = origin;
        }

        _frames.push_back(frame);

        return _frames.size() - 1;
    }

    const iTexturePtr &iSprite::getTexture() const
    {
        return _texture;
    }

    uint32 iSprite::getFrameCount() const
    {
        return static_cast<uint32>(_frames.size());
    }

    const iSprite::iFrame &iSprite::getFrame(uint32 index) const
    {
        con_assert(index < _frames.size(), "out of range " << index << " - " << _frames.size());

        return _frames[index];
    }
}; // namespace igor
