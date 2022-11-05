// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/texture/iTexture.h>

#include <igor/renderer/utils/iRendererUtils.h>

namespace igor
{

	iTexture::iTexture(iaString name, iResourceCacheMode cacheMode, iTextureBuildMode buildMode, iTextureWrapMode wrapMode)
		: _filename(name), _buildMode(buildMode), _wrapMode(wrapMode), _cacheMode(cacheMode)
	{
	}

	iTexture::~iTexture()
	{
	}

	void iTexture::bind(uint32 textureUnit)
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		GL_CHECK_ERROR();
		glBindTexture(GL_TEXTURE_2D, _textureID);
		GL_CHECK_ERROR();
	}

	uint32 iTexture::getTextureID() const
	{
		return _textureID;
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

	const iaString &iTexture::getFilename() const
	{
		return _filename;
	}

}; // namespace igor