// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iSpriteRenderComponent.h>

namespace igor
{
    iSpriteRenderComponent::iSpriteRenderComponent(iSpritePtr sprite, const iaVector2d &size, const iaColor4f &color, int32 zIndex, iRenderMode renderMode)
        : _sprite(sprite), _size(size), _color(color), _zIndex(zIndex), _renderMode(renderMode)
    {
    }

    iEntityComponent *iSpriteRenderComponent::createInstance()
    {
        return new iSpriteRenderComponent();
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

}