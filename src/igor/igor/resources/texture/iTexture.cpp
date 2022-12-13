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
        con_assert(glIsTexture(_textureID), "invalid texture id " << _textureID);
        glBindTextureUnit(textureUnit, _textureID);
        GL_CHECK_ERROR();
    }

    uint32 iTexture::getTextureID() const
    {
        return _textureID;
    }

    bool iTexture::isProcessed() const
    {
        return _processed;
    }

    bool iTexture::isValid() const
    {
        return _valid;
    }

    int32 iTexture::getWidth() const
    {
        return _width;
    }

    int32 iTexture::getHeight() const
    {
        return _height;
    }

    int32 iTexture::getBpp() const
    {
        return _bpp;
    }

    iColorFormat iTexture::getColorFormat() const
    {
        return _colorFormat;
    }

    bool iTexture::hasTransparency() const
    {
        return _hasTrans;
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

    bool iTexture::useFallback() const
    {
        return _useFallback;
    }

    void iTexture::setData(int32 width, int32 height, int32 bytepp, iColorFormat format, unsigned char *data, iTextureBuildMode buildMode, iTextureWrapMode wrapMode)
    {
        const int32 glformat = iRendererUtils::convertType(format, false);
        const int32 glformatSized = iRendererUtils::convertType(format, true);

        _width = width;
        _height = height;
        _colorFormat = format;
        _bpp = bytepp;

        if (format == iColorFormat::RGBA)
        {
            const uint32 dataSize = _width * _height * _bpp;

            for(int i=3;i<dataSize;i+=4)
            {
                if(data[i] != 0xff)
                {
                    _hasTrans = true;
                    break;
                }
            }
        }

        glCreateTextures(GL_TEXTURE_2D, 1, &_textureID);
        GL_CHECK_ERROR();

        switch (wrapMode)
        {
        case iTextureWrapMode::Repeat:
            glTextureParameteri(_textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
            GL_CHECK_ERROR();
            glTextureParameteri(_textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
            GL_CHECK_ERROR();
            break;

        case iTextureWrapMode::Clamp:
            glTextureParameteri(_textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            GL_CHECK_ERROR();
            glTextureParameteri(_textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            GL_CHECK_ERROR();
            break;

        case iTextureWrapMode::MirrorRepeat:
            glTextureParameteri(_textureID, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            GL_CHECK_ERROR();
            glTextureParameteri(_textureID, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            GL_CHECK_ERROR();
            break;
        }

        glTextureParameteri(_textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        GL_CHECK_ERROR();

        if (buildMode == iTextureBuildMode::Mipmapped)
        {
            _mipMapLevels = floor(log2(std::max(width,height)))+1;
        }

        // TODO remove glTexImage2D
        //glTextureStorage2D(_textureID, _mipMapLevels, glformatSized, _width, _height);
        //GL_CHECK_ERROR();

        // glTextureSubImage2D(_textureID, 0, 0, 0, _width, _height, glformat, GL_UNSIGNED_BYTE, data);
        //GL_CHECK_ERROR();

        glBindTexture(GL_TEXTURE_2D, _textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, glformatSized, _width, _height, 0, glformat, GL_UNSIGNED_BYTE, data);
        GL_CHECK_ERROR();

        if (buildMode == iTextureBuildMode::Mipmapped)
        {
            glTextureParameteri(_textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            GL_CHECK_ERROR();            
            glGenerateTextureMipmap(_textureID);
            GL_CHECK_ERROR();
        }
        else
        {
            glTextureParameterf(_textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
    }

    uint32 iTexture::getMipMapLevels() const
    {
        return _mipMapLevels;
    }

}; // namespace igor