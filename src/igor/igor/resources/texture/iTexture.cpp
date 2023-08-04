// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/texture/iTexture.h>
#include <igor/resources/iResourceManager.h>

#include <igor/renderer/utils/iRendererUtils.h>

#ifdef IGOR_WINDOWS
// glu needs this under windows
#include <windows.h>
#endif
#include <GL/glu.h>

namespace igor
{

    iTexture::iTexture(const iParameters &parameters)
        : iResource("texture", parameters)
    {
        _wrapMode = parameters.getParameter<iTextureWrapMode>("wrapMode", iTextureWrapMode::Repeat);
        _buildMode = parameters.getParameter<iTextureBuildMode>("buildMode", iTextureBuildMode::Mipmapped);
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

    iTextureWrapMode iTexture::getWrapMode() const
    {
        return _wrapMode;
    }

    iTextureBuildMode iTexture::getBuildMode() const
    {
        return _buildMode;
    }

    bool iTexture::useFallback() const
    {
        return _useFallback;
    }

    uint32 iTexture::calcMipMapLevels(int32 width, int32 height)
    {
        return 1 + std::floor(std::log2(std::max(width, height)));
    }

    static bool detectTransparency(int32 width, int32 height, int32 bytepp, iColorFormat format, unsigned char *data)
    {
        if (format != iColorFormat::RGBA)
        {
            return false;
        }

        const int32 dataSize = width * height * bytepp;

        for (int i = 3; i < dataSize; i += 4)
        {
            if (data[i] != 0xff)
            {
                return true;
            }
        }

        return false;
    }

    void iTexture::setData(int32 width, int32 height, int32 bytepp, iColorFormat format, unsigned char *data, iTextureBuildMode buildMode, iTextureWrapMode wrapMode)
    {
        const int32 glformat = iRendererUtils::convertType(format, false);
        const int32 glformatSized = iRendererUtils::convertType(format, true);

        _buildMode = buildMode;
        _wrapMode = wrapMode;
        _width = width;
        _height = height;
        _colorFormat = format;
        _bpp = bytepp;
        _hasTrans = detectTransparency(width, height, bytepp, format, data);

        glCreateTextures(GL_TEXTURE_2D, 1, &_textureID);
        GL_CHECK_ERROR();

        switch (_wrapMode)
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

        if (_buildMode == iTextureBuildMode::Mipmapped)
        {
            _mipMapLevels = calcMipMapLevels(width, height);
            glTextureParameteri(_textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            GL_CHECK_ERROR();
            glTextureParameteri(_textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            GL_CHECK_ERROR();

            con_trace("create mipmap for:" << getName() << " id:" << _textureID << " levels:" << _mipMapLevels << " width:" << _width << " height:" << _height << " format:" << format);
            glTextureStorage2D(_textureID, _mipMapLevels, glformatSized, _width, _height);
            GL_CHECK_ERROR();
            glTextureSubImage2D(_textureID, 0, 0, 0, _width, _height, glformat, GL_UNSIGNED_BYTE, data);
            GL_CHECK_ERROR();
            glGenerateTextureMipmap(_textureID);
            GL_CHECK_ERROR();
        }
        else
        {
            glTextureParameterf(_textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            GL_CHECK_ERROR();
            glTextureParameteri(_textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            GL_CHECK_ERROR();

            glTextureStorage2D(_textureID, 1, glformatSized, _width, _height);
            GL_CHECK_ERROR();
            glTextureSubImage2D(_textureID, 0, 0, 0, _width, _height, glformat, GL_UNSIGNED_BYTE, data);
            GL_CHECK_ERROR();
        }
    }

    uint32 iTexture::getMipMapLevels() const
    {
        return _mipMapLevels;
    }

}; // namespace igor