// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iSprite.h>

#include <iTextureResourceFactory.h>

namespace Igor
{

	iSprite::iSprite(iTexturePtr texture)
	{
		_texture = texture;
	}

	void iSprite::setTexture(iTexturePtr texture)
	{
		_texture = texture;
		// TODO recalc frames if texture sizes differ
	}

	uint32 iSprite::addFrame(const iaVector2f& pos, const iaVector2f& size, const iaVector2f& origin, bool pixel)
	{
		Frame frame;

		if (pixel)
		{
			frame._pos.set(pos._x / _texture->getWidth(), pos._y / _texture->getHeight());
			frame._size.set(size._x / _texture->getWidth(), size._y / _texture->getHeight());
			frame._origin = origin;
		}
		else
		{
			frame._pos = pos;
			frame._size = size;
			frame._origin.set(origin._x * _texture->getWidth(), origin._y * _texture->getHeight());
		}

		_frames.push_back(frame);

		return _frames.size() - 1;
	}

};
