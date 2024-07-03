// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iSpriteRenderComponent.h>

namespace igor
{
    iSpriteRenderComponent::iSpriteRenderComponent(iSpritePtr sprite, const iaVector2d &size, const iaColor4f &color, int32 zIndex, iRenderMode renderMode, const iaString &name)
        : iEntityComponent(name), _sprite(sprite), _size(size), _color(color), _zIndex(zIndex), _renderMode(renderMode)
    {
    }
}