// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/texture/iTextureFont.h>
#include <igor/resources/texture/iPixmap.h>
#include <igor/resources/texture/iTextureResourceFactory.h>
#include <igor/resources/iResourceManager.h>

namespace igor
{

#define CHARACTERINTEXWIDTH 16
#define CHARACTERINTEXHEIGHT 8

    iTextureFontPtr iTextureFont::create(const iaString &filename, iFontType type, iColorMask colorMask, float32 colorMaskThreashold)
    {
        return std::make_shared<iTextureFont>(filename, type, colorMask, colorMaskThreashold);
    }

    iTextureFont::iTextureFont(const iaString &filename, iFontType type, iColorMask colorMask, float32 colorMaskThreashold)
    {
        valid = false;

        const iaString resolved = iResourceManager::getInstance().getPath(filename);

        _texture = iTextureResourceFactory::getInstance().loadFile(resolved, iResourceCacheMode::Free, iTextureBuildMode::Normal);

        if (!_texture->isValid())
        {
            return;
        }
        _pixmap = iTextureResourceFactory::getInstance().loadPixmap(resolved);

        if (_pixmap == nullptr)
        {
            return;
        }

        //! \todo this depends on the _texture color format
        uint8 borderchannel = 0;
        switch (colorMask)
        {
        case iColorMask::Red:
            borderchannel = 0;
            break;
        case iColorMask::Green:
            borderchannel = 1;
            break;
        case iColorMask::Blue:
            borderchannel = 2;
            break;
        case iColorMask::Alpha:
            borderchannel = 3;
            break;
        };

        iCharacterDimensions tempchar;

        for (unsigned short i = 0; i < CHARACTERINTEXWIDTH * CHARACTERINTEXHEIGHT; i++)
        {
            _characters.push_back(tempchar);
        }

        long offx = _pixmap->getWidth() / CHARACTERINTEXWIDTH;
        long offy = _pixmap->getHeight() / CHARACTERINTEXHEIGHT;

        uint32 character = 0;

        bool firstfound;

        bool fixed_width = false;

        for (long tex_y = 0; tex_y < CHARACTERINTEXHEIGHT; tex_y++)
        {
            for (long tex_x = 0; tex_x < CHARACTERINTEXWIDTH; tex_x++)
            {
                _characters[character]._characterRect.setY(float32(tex_y * offy));
                _characters[character]._characterRect.setHeight(float32(offy));

                _characters[character]._characterRect.setX(float32(tex_x * offx));

                if (!fixed_width)
                {
                    firstfound = false;
                    for (long pos_x = tex_x * offx; pos_x < tex_x * offx + offx; pos_x++)
                    {
                        for (long pos_y = tex_y * offy; pos_y < tex_y * offy + offy; pos_y++)
                        {
                            if (_pixmap->getPixel(pos_x, pos_y, borderchannel) > colorMaskThreashold)
                            {
                                if (!firstfound)
                                {
                                    _characters[character]._characterRect.setX((float32)pos_x - 1);
                                    firstfound = true;
                                }
                            }
                        }
                    }
                }

                _characters[character]._characterRect.setWidth((float32)tex_x * offx + offx - _characters[character]._characterRect.getX());

                if (!fixed_width)
                {
                    float32 reducedWidth = _characters[character]._characterRect.getWidth() / 3.0f;
                    _characters[character]._characterRect.setWidth(reducedWidth); // damit freizeichen nicht �bertrieben breit sind

                    firstfound = false;
                    for (long pos_x = tex_x * offx + offx - 1; pos_x >= tex_x * offx; pos_x--)
                    {
                        for (long pos_y = tex_y * offy; pos_y < tex_y * offy + offy; pos_y++)
                        {
                            if (_pixmap->getPixel(pos_x, pos_y, 3) != 0)
                            {
                                if (!firstfound)
                                {
                                    _characters[character]._characterRect.setWidth(pos_x - (float32)_characters[character]._characterRect.getX() + 2);
                                    firstfound = true;
                                }
                            }
                        }
                    }
                }

                _characters[character]._characterOffset = _characters[character]._characterRect.getWidth() / (float32)offx;

                _characters[character]._characterRect.setX(_characters[character]._characterRect.getX() / (float32)_pixmap->getWidth());
                _characters[character]._characterRect.setWidth(_characters[character]._characterRect.getWidth() / (float32)_pixmap->getWidth());
                _characters[character]._characterRect.setY(_characters[character]._characterRect.getY() / (float32)_pixmap->getHeight());
                _characters[character]._characterRect.setHeight(_characters[character]._characterRect.getHeight() / (float32)_pixmap->getHeight());

                character++;
            }
        }

        // no need to old on to this
        delete _pixmap;
        _pixmap = nullptr;        

        switch (type)
        {
        case iFontType::Standard:
            break;

        case iFontType::FixedWidth:
            makeFixedWidth();
            break;

        case iFontType::FixedDigitWidth:
            makeFixedDigitWidth();
            break;
        }

        valid = true;
    }

    iTextureFont::~iTextureFont()
    {
        _texture = nullptr;
        _characters.clear();
    }

    iTexturePtr iTextureFont::getTexture()
    {
        return _texture;
    }

    const std::vector<iCharacterDimensions> &iTextureFont::getCharacters() const
    {
        return _characters;
    }

    void iTextureFont::makeFixedWidth()
    {
        float32 maxWidth = 0;
        float32 characterOffset = 0;

        for (uint32 i = 0; i < _characters.size(); i++)
        {
            if (_characters[i]._characterRect.getWidth() > maxWidth)
            {
                maxWidth = _characters[i]._characterRect.getWidth();
                characterOffset = _characters[i]._characterOffset;
            }
        }

        for (uint32 i = 0; i < _characters.size(); i++)
        {
            modifyWidth(_characters[i], maxWidth, characterOffset);
        }
    }

    void iTextureFont::makeFixedDigitWidth()
    {
        float32 maxdigitwidth = 0;
        float32 temp_render_width_relative = 0;

        for (unsigned char i = 8; i <= 30; i++)
        {
            if (_characters[i]._characterRect.getWidth() > maxdigitwidth)
            {
                maxdigitwidth = _characters[i]._characterRect.getWidth();
                temp_render_width_relative = _characters[i]._characterOffset;
            }
        }

        if (_characters[0]._characterRect.getWidth() > maxdigitwidth) // Freizeichen
        {
            maxdigitwidth = _characters[0]._characterRect.getWidth();
            temp_render_width_relative = _characters[0]._characterOffset;
        }

        for (unsigned char i = 8; i <= 30; i++)
        {
            modifyWidth(_characters[i], maxdigitwidth, temp_render_width_relative);
        }

        modifyWidth(_characters[0], maxdigitwidth, temp_render_width_relative); // Freizeichen
    }

    void iTextureFont::modifyWidth(iCharacterDimensions &character, float32 newWidth, float32 newCharacterOffset)
    {
        float32 tempwidth = character._characterRect.getWidth();
        character._characterRect.setWidth(newWidth);
        tempwidth -= newWidth;
        tempwidth *= 0.5f;
        character._characterRect.setX(character._characterRect.getX() + tempwidth);
        character._characterOffset = newCharacterOffset;
    }

    bool iTextureFont::isValid()
    {
        return valid;
    }

    float32 iTextureFont::measureWidth(iaString text, float32 size)
    {
        if (valid)
        {
            float32 lenght = 0;
            for (uint32 i = 0; i < text.getLength(); i++)
            {
                uint32 index = (uint32)text[i] - 32;

                if(index < 0 || 
                index > _characters.size())
                {
                    continue;
                }

                lenght += _characters[index]._characterOffset * size;
            }
            return lenght;
        }

        return 0;
    }

    float32 iTextureFont::measureHeight(iaString text, float32 size, float32 maxWidth, float32 lineHeight)
    {
        float32 height = 0;

        if (maxWidth == 0.0f)
        {
            return size;
        }

        if (valid && text != L"")
        {
            float32 length = 0;
            float32 lastlength = 0;

            for (uint32 i = 0; i < text.getLength(); i++)
            {
                length += _characters[((unsigned char)text[i]) - 32]._characterOffset * size;
                if ((unsigned char)text[i] == ' ')
                {
                    if (length > maxWidth)
                    {
                        length = length - lastlength;
                        height += lineHeight * size;
                    }

                    lastlength = length;
                }
            }

            if (length > maxWidth)
            {
                height += lineHeight * size;
            }

            height += lineHeight * size;
        }

        return height;
    }

}; // namespace igor
