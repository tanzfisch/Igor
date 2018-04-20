// Igor game engine
// (c) Copyright 2012-2018 by Martin Loga
// see copyright notice in corresponding header file

#include <iTexture.h>

namespace Igor
{

	iTexture::iTexture(iaString name, iResourceCacheMode cacheMode, iTextureBuildMode buildMode, iTextureWrapMode wrapMode)
		: _filename(name)
		, _buildMode(buildMode)
        , _wrapMode(wrapMode)
        , _cacheMode(cacheMode)
	{
	}

	iTexture::~iTexture()
	{
	}

    bool iTexture::isProcessed()
    {
        return _processed;
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

    iResourceCacheMode iTexture::getCacheMode() const
    {
        return _cacheMode;
    }

    iTextureWrapMode iTexture::getWrapMode() const
    {
        return _wrapMode;
    }

	iTextureBuildMode iTexture::getBuildMode() const
	{
		return _buildMode;
	}

	iaString iTexture::getFilename()
	{
		return _filename;
	}

};