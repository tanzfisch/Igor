// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/texture/iAtlas.h>

#include <igor/resources/texture/iTextureResourceFactory.h>

#include <tinyxml.h>

namespace igor
{

	iAtlas::iAtlas(iTexturePtr texture)
	{
		_texture = texture;
	}

	void iAtlas::setTexture(iTexturePtr texture)
	{
		_texture = texture;
		// TODO recalc frames if texture sizes differ
	}

	uint32 iAtlas::addFrame(const iaVector2f &pos, const iaVector2f &size, const iaVector2f &origin, bool pixel)
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
		char temp[2048];
		filename.getData(temp, 2048);

		TiXmlDocument document(temp);
		document.LoadFile();

		TiXmlElement *root = document.FirstChildElement("Igor");
		if (root)
		{
			TiXmlElement *atlas = root->FirstChildElement("Atlas");
			if (atlas)
			{
				readAtlas(atlas);
			}
		}
	}
}; // namespace igor
