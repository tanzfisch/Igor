// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iTextureFont.h>
#include <iPixmap.h>
#include <iTextureResourceFactory.h>
#include <iResourceManager.h>

namespace Igor
{

#define CHARACTERINTEXWIDTH 16
#define CHARACTERINTEXHEIGHT 8

    iTextureFont::iTextureFont(iaString font_file, iFontType font_type, iColorMask mask_channels, float32 mask_threashold)
    {
        valid = false;

        font_file = iResourceManager::getInstance().getPath(font_file);
        _texture = iTextureResourceFactory::getInstance().loadFile(font_file, iResourceCacheMode::Free, iTextureBuildMode::Normal);

        if (_texture->isValid())
        {
            _pixmap = iTextureResourceFactory::getInstance().loadFileAsPixmap(font_file);

            if (_pixmap)
            {
                //! \todo this depends on the _texture color format
                uint8 borderchannel = 0;
                switch (mask_channels)
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

                for (unsigned short i = 0; i < CHARACTERINTEXWIDTH*CHARACTERINTEXHEIGHT; i++)
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
                        _characters[character].rect.setY(float32(tex_y*offy));
                        _characters[character].rect.setHeight(float32(offy));

                        _characters[character].rect.setX(float32(tex_x*offx));

                        if (!fixed_width)
                        {
                            firstfound = false;
                            for (long pos_x = tex_x*offx; pos_x < tex_x*offx + offx; pos_x++)
                            {
                                for (long pos_y = tex_y*offy; pos_y<tex_y*offy + offy; pos_y++)
                                {
                                    if (_pixmap->getPixel(pos_x, pos_y, borderchannel)>mask_threashold)
                                    {
                                        if (!firstfound)
                                        {
                                            _characters[character].rect.setX((float32)pos_x - 1);
                                            firstfound = true;
                                        }
                                    }
                                }
                            }
                        }

                        _characters[character].rect.setWidth((float32)tex_x*offx + offx - _characters[character].rect.getX());

                        if (!fixed_width)
                        {
                            float32 reducedWidth = _characters[character].rect.getWidth() / 3.0f;
                            _characters[character].rect.setWidth(reducedWidth); // damit freizeichen nicht übertrieben breit sind

                            firstfound = false;
                            for (long pos_x = tex_x*offx + offx - 1; pos_x >= tex_x*offx; pos_x--)
                            {
                                for (long pos_y = tex_y*offy; pos_y < tex_y*offy + offy; pos_y++)
                                {
                                    if (_pixmap->getPixel(pos_x, pos_y, 3) != 0)
                                    {
                                        if (!firstfound)
                                        {
                                            _characters[character].rect.setWidth(pos_x - (float32)_characters[character].rect.getX() + 2);
                                            firstfound = true;
                                        }
                                    }
                                }
                            }
                        }

                        _characters[character].relRenderWidth = _characters[character].rect.getWidth() / (float32)offx;

                        _characters[character].rect.setX(_characters[character].rect.getX() / (float32)_pixmap->getWidth());
                        _characters[character].rect.setWidth(_characters[character].rect.getWidth() / (float32)_pixmap->getWidth());
                        _characters[character].rect.setY(_characters[character].rect.getY() / (float32)_pixmap->getHeight());
                        _characters[character].rect.setHeight(_characters[character].rect.getHeight() / (float32)_pixmap->getHeight());

                        character++;
                    }
                }

                switch (font_type)
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

                if (_pixmap)
                {
                    delete _pixmap;
                    _pixmap = 0;
                }

                valid = true;
            }
        }
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

    std::vector<iCharacterDimensions>& iTextureFont::getCharacters()
    {
        return _characters;
    }

    void iTextureFont::makeFixedWidth()
    {
        float32 maxWidth = 0;
        float32 relRenderWidth = 0;

        for (uint32 i = 0; i<_characters.size(); i++)
        {
            if (_characters[i].rect.getWidth() > maxWidth)
            {
                maxWidth = _characters[i].rect.getWidth();
                relRenderWidth = _characters[i].relRenderWidth;
            }
        }

        for (uint32 i = 0; i<_characters.size(); i++)
        {
            modifyWidth(_characters[i], maxWidth, relRenderWidth);
        }
    }

    void iTextureFont::makeFixedDigitWidth()
    {
        float32 maxdigitwidth = 0;
        float32 temp_render_width_relative = 0;

        for (unsigned char i = 8; i <= 30; i++)
        {
            if (_characters[i].rect.getWidth() > maxdigitwidth)
            {
                maxdigitwidth = _characters[i].rect.getWidth();
                temp_render_width_relative = _characters[i].relRenderWidth;
            }
        }

        if (_characters[0].rect.getWidth() > maxdigitwidth) // Freizeichen
        {
            maxdigitwidth = _characters[0].rect.getWidth();
            temp_render_width_relative = _characters[0].relRenderWidth;
        }

        for (unsigned char i = 8; i <= 30; i++)
        {
            modifyWidth(_characters[i], maxdigitwidth, temp_render_width_relative);
        }

        modifyWidth(_characters[0], maxdigitwidth, temp_render_width_relative); // Freizeichen
    }

    void iTextureFont::modifyWidth(iCharacterDimensions &character, float32 newWidth, float32 newRelRenderWidth)
    {
        float32 tempwidth = character.rect.getWidth();
        character.rect.setWidth(newWidth);
        tempwidth -= newWidth;
        tempwidth *= 0.5f;
        character.rect.setX(character.rect.getX() + tempwidth);
        character.relRenderWidth = newRelRenderWidth;
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
            for (uint32 i = 0; i < text.getSize(); i++)
            {
                if (text[i] > 0 &&
                    text[i] < _characters.size())
                {
                    lenght += _characters[((unsigned char)text[i]) - 32].relRenderWidth*size;
                }
            }
            return lenght;
        }

        return 0;
    }

    float32 iTextureFont::measureHeight(iaString text, float32 size, float32 maxWidth, float32 lineHeight)
    {
        float32 height = 0;

        if (maxWidth == 0.0f) return size;

        if (valid && text != L"")
        {
            float32 length = 0;
            float32 lastlength = 0;

            for (uint32 i = 0; i < text.getSize(); i++)
            {
                length += _characters[((unsigned char)text[i]) - 32].relRenderWidth*size;
                if ((unsigned char)text[i] == ' ')
                {
                    if (length > maxWidth)
                    {
                        length = length - lastlength;
                        height += lineHeight*size;
                    }

                    lastlength = length;
                }
            }

            if (length > maxWidth)
            {
                height += lineHeight*size;
            }

            height += lineHeight*size;
        }

        return height;
    }

};
