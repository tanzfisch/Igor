// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iSpriteRenderComponent.h>

namespace igor
{
    iSpriteRenderComponent::iSpriteRenderComponent(iSpritePtr sprite, const iaVector2d &size, const iaColor4f &color, int32 zIndex, iRenderMode renderMode)
        : _sprite(sprite), _size(size), _color(color), _zIndex(zIndex), _renderMode(renderMode)
    {
    }

    iEntityComponentPtr iSpriteRenderComponent::createInstance()
    {
        return new iSpriteRenderComponent();
    }

    const iaString &iSpriteRenderComponent::getTypeName()
    {
        static const iaString name("igor_sprite_render_component");
        return name;
    }

    iEntityComponentPtr iSpriteRenderComponent::getCopy()
    {
        iSpriteRenderComponent *component = new iSpriteRenderComponent();
        component->_sprite = _sprite;
        component->_size = _size;
        component->_color = _color;
        component->_zIndex = _zIndex;
        component->_renderMode = _renderMode;
        component->_frameIndex = _frameIndex;
        return component;
    }

    std::vector<iaString> iSpriteRenderComponent::getInfo() const
    {
        std::vector<iaString> result = iEntityComponent::getInfo();

        if (_sprite)
        {
            result.push_back(iaString("Spt: ") + _sprite->getID());
        }
        else
        {
            result.push_back(iaString("no sprite"));
        }

        result.push_back(iaString("ZIx: ") + iaString::toString(_zIndex));

        return result;
    }

    void iSpriteRenderComponent::setSprite(iSpritePtr sprite)
    {
        _sprite = sprite;
        increaseVersion();
    }

    void iSpriteRenderComponent::setSize(const iaVector2d &size)
    {
        _size = size;
        increaseVersion();
    }

    void iSpriteRenderComponent::setColor(const iaColor4f &color)
    {
        _color = color;
        increaseVersion();
    }

    void iSpriteRenderComponent::setZIndex(int32 zIndex)
    {
        _zIndex = zIndex;
        increaseVersion();
    }

    void iSpriteRenderComponent::setRenderMode(iSpriteRenderComponent::iRenderMode mode)
    {
        _renderMode = mode;
        increaseVersion();
    }

    void iSpriteRenderComponent::setFrameIndex(int32 frameIndex)
    {
        _frameIndex = frameIndex;
        increaseVersion();
    }
}