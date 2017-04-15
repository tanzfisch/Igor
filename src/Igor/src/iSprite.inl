// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

__IGOR_INLINE__ shared_ptr<iTexture> iSprite::getTexture() const
{
    return _texture;
}

__IGOR_INLINE__ const iaVector2f& iSprite::getTexCoord(uint32 index) const
{
    return _texCoord[index];
}

__IGOR_INLINE__  const iaVector2f& iSprite::getOrigin() const
{
    return _origin;
}

__IGOR_INLINE__ float32 iSprite::getWidth() const
{
    return _width;
}

__IGOR_INLINE__ float32 iSprite::getHeight() const
{
    return _height;
}
