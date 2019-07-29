// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

__IGOR_INLINE__ iTexturePtr iSprite::getTexture() const
{
    return _texture;
}

__IGOR_INLINE__  uint32 iSprite::getFrameCount() const
{
	return static_cast<uint32>(_frames.size());
}

__IGOR_INLINE__ const iSprite::Frame& iSprite::getFrame(uint32 index) const
{
	con_assert(index < _frames.size(),"out of range");

	return _frames[index];
}