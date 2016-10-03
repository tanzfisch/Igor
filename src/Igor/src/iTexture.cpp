// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iTexture.h>

namespace Igor
{

	iTexture::iTexture(iaString name, iTextureBuildMode buildMode, iTextureWrapMode wrapMode)
		: _filename(name)
		, _buildMode(buildMode)
        , _wrapMode(wrapMode)
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

    iTextureWrapMode iTexture::getTextureWrapMode() const
    {
        return _wrapMode;
    }

	iTextureBuildMode iTexture::getTextureBuildMode() const
	{
		return _buildMode;
	}

	iaString iTexture::getFilename()
	{
		return _filename;
	}

};