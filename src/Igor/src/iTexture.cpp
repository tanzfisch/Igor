// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iTexture.h>

namespace Igor
{

	iTexture::iTexture(iaString name, iTextureBuildMode mode)
		: _filename(name)
		, _mode(mode)
	{
	}

	iTexture::~iTexture()
	{
	}

	bool iTexture::isValid()
	{
		return _valid;
	}

	bool iTexture::isDummy()
	{
		return _dummy;
	}

	int32 iTexture::getWidth()
	{
		return _width;
	}

	int32 iTexture::getHeight()
	{
		return _height;
	}

	int32 iTexture::getBpp()
	{
		return _bpp;
	}

	iColorFormat iTexture::getColorFormat()
	{
		return _colorFormat;
	}

	iTextureBuildMode iTexture::getTextureBuildMode()
	{
		return _mode;
	}

	iaString iTexture::getFilename()
	{
		return _filename;
	}

};