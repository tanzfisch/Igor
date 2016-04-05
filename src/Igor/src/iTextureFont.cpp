// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
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
        texture = iTextureResourceFactory::getInstance().loadFile(font_file, iTextureBuildMode::Normal);

        if (texture->isValid())
        {
            pixmap = iTextureResourceFactory::getInstance().loadFileAsPixmap(font_file);

            if (pixmap)
            {
                //! \todo this depends on the texture color format
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
                    characters.push_back(tempchar);
                }

                long offx = pixmap->getWidth() / CHARACTERINTEXWIDTH;
                long offy = pixmap->getHeight() / CHARACTERINTEXHEIGHT;

                uint32 character = 0;

                bool firstfound;

                bool fixed_width = false;

                for (long tex_y = 0; tex_y < CHARACTERINTEXHEIGHT; tex_y++)
                {
                    for (long tex_x = 0; tex_x < CHARACTERINTEXWIDTH; tex_x++)
                    {
                        characters[character].rect.setY(float32(tex_y*offy));
                        characters[character].rect.setHeight(float32(offy));

                        characters[character].rect.setX(float32(tex_x*offx));

                        if (!fixed_width)
                        {
                            firstfound = false;
                            for (long pos_x = tex_x*offx; pos_x < tex_x*offx + offx; pos_x++)
                            {
                                for (long pos_y = tex_y*offy; pos_y<tex_y*offy + offy; pos_y++)
                                {
                                    if (pixmap->getPixel(pos_x, pos_y, borderchannel)>mask_threashold)
                                    {
                                        if (!firstfound)
                                        {
                                            characters[character].rect.setX((float32)pos_x - 1);
                                            firstfound = true;
                                        }
                                    }
                                }
                            }
                        }

                        characters[character].rect.setWidth((float32)tex_x*offx + offx - characters[character].rect.getX());

                        if (!fixed_width)
                        {
                            float32 reducedWidth = characters[character].rect.getWidth() / 3.0f;
                            characters[character].rect.setWidth(reducedWidth); // damit freizeichen nicht übertrieben breit sind

                            firstfound = false;
                            for (long pos_x = tex_x*offx + offx; pos_x > tex_x*offx; pos_x--)
                            {
                                for (long pos_y = tex_y*offy; pos_y < tex_y*offy + offy; pos_y++)
                                {
                                    if (pixmap->getPixel(pos_x, pos_y, 3) != 0)
                                    {
                                        if (!firstfound)
                                        {
                                            characters[character].rect.setWidth(pos_x - (float32)characters[character].rect.getX() + 2);
                                            firstfound = true;
                                        }
                                    }
                                }
                            }
                        }

                        characters[character].relRenderWidth = characters[character].rect.getWidth() / (float32)offx;

                        characters[character].rect.setX(characters[character].rect.getX() / (float32)pixmap->getWidth());
                        characters[character].rect.setWidth(characters[character].rect.getWidth() / (float32)pixmap->getWidth());
                        characters[character].rect.setY(characters[character].rect.getY() / (float32)pixmap->getHeight());
                        characters[character].rect.setHeight(characters[character].rect.getHeight() / (float32)pixmap->getHeight());

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

                if (pixmap)
                {
                    delete pixmap;
                    pixmap = 0;
                }

                valid = true;
            }
        }
    }


    iTextureFont::~iTextureFont()
    {
        texture = 0;
        characters.clear();
    }

    shared_ptr<iTexture> iTextureFont::getTexture()
    {
        return texture;
    }

    vector<iCharacterDimensions>& iTextureFont::getCharacters()
    {
        return characters;
    }

    void iTextureFont::makeFixedWidth()
    {
        float32 maxWidth = 0;
        float32 relRenderWidth = 0;

        for (uint32 i = 0; i<characters.size(); i++)
        {
            if (characters[i].rect.getWidth() > maxWidth)
            {
                maxWidth = characters[i].rect.getWidth();
                relRenderWidth = characters[i].relRenderWidth;
            }
        }

        for (uint32 i = 0; i<characters.size(); i++)
        {
            modifyWidth(characters[i], maxWidth, relRenderWidth);
        }
    }

    void iTextureFont::makeFixedDigitWidth()
    {
        float32 maxdigitwidth = 0;
        float32 temp_render_width_relative = 0;

        for (unsigned char i = 8; i <= 30; i++)
        {
            if (characters[i].rect.getWidth() > maxdigitwidth)
            {
                maxdigitwidth = characters[i].rect.getWidth();
                temp_render_width_relative = characters[i].relRenderWidth;
            }
        }

        if (characters[0].rect.getWidth() > maxdigitwidth) // Freizeichen
        {
            maxdigitwidth = characters[0].rect.getWidth();
            temp_render_width_relative = characters[0].relRenderWidth;
        }

        for (unsigned char i = 8; i <= 30; i++)
        {
            modifyWidth(characters[i], maxdigitwidth, temp_render_width_relative);
        }

        modifyWidth(characters[0], maxdigitwidth, temp_render_width_relative); // Freizeichen
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
                    text[i] < characters.size())
                {
                    lenght += characters[((unsigned char)text[i]) - 32].relRenderWidth*size;
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
                length += characters[((unsigned char)text[i]) - 32].relRenderWidth*size;
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
