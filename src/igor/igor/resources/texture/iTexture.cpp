// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/texture/iTexture.h>

#include <igor/renderer/utils/iRendererUtils.h>

#ifdef __IGOR_WINDOWS__
// glu needs this under windows
#include <windows.h>
#endif
#include <GL/glu.h>

namespace igor
{

    void deleter(const iTexture *texture)
    {
        delete texture;
    }

    iTexturePtr iTexture::create(iaString name, iResourceCacheMode cacheMode, iTextureBuildMode buildMode, iTextureWrapMode wrapMode)
    {
        return std::shared_ptr<iTexture>(new iTexture(name, cacheMode, buildMode, wrapMode), deleter);
    }

	iTexture::iTexture(iaString name, iResourceCacheMode cacheMode, iTextureBuildMode buildMode, iTextureWrapMode wrapMode)
		: _filename(name), _buildMode(buildMode), _wrapMode(wrapMode), _cacheMode(cacheMode)
	{
	}

	void iTexture::bind(uint32 textureUnit)
	{
		glBindTextureUnit(textureUnit, _textureID);
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

    void iTexture::setData(int32 width, int32 height, int32 bytepp, iColorFormat format, unsigned char *data, iTextureBuildMode buildMode, iTextureWrapMode wrapMode)
    {
        int32 glformat = iRendererUtils::convertType(format);
        con_assert(glformat != -1, "invalid color format type");

        _width = width;
        _height = height;
        _colorFormat = format;
        _bpp = bytepp;

        glGenTextures(1, &_textureID);
        GL_CHECK_ERROR();
        glBindTexture(GL_TEXTURE_2D, _textureID);
        GL_CHECK_ERROR();

        switch (wrapMode)
        {
        case iTextureWrapMode::Repeat:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            GL_CHECK_ERROR();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            GL_CHECK_ERROR();
            break;

        case iTextureWrapMode::Clamp:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            GL_CHECK_ERROR();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            GL_CHECK_ERROR();
            break;

        case iTextureWrapMode::MirrorRepeat:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            GL_CHECK_ERROR();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            GL_CHECK_ERROR();
            break;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        GL_CHECK_ERROR();

        if (buildMode == iTextureBuildMode::Normal)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            GL_CHECK_ERROR();
            glTexImage2D(GL_TEXTURE_2D, 0, bytepp, width, height, 0, glformat, GL_UNSIGNED_BYTE, data);
            GL_CHECK_ERROR();
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            GL_CHECK_ERROR();
            gluBuild2DMipmaps(GL_TEXTURE_2D, bytepp, width, height, glformat, GL_UNSIGNED_BYTE, data);
            GL_CHECK_ERROR();
        }
    }

}; // namespace igor