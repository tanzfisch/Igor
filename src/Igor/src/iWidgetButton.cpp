// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetButton.h>

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iTextureFont.h>
#include <iTextureResourceFactory.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
	iWidgetButton::iWidgetButton()
		: iWidget(iWidgetType::Button)
	{
		_height = 20;
		_width = 60;
	}

    iWidgetButton::~iWidgetButton()
    {
        _texture = nullptr;
    }

	void iWidgetButton::setText(const iaString& text)
	{
		_text = text;
		update();
	}

	const iaString& iWidgetButton::getText() const
	{
		return _text;
	}

    void iWidgetButton::setTexture(const iaString& texturePath)
    {
        if (_texturePath != texturePath)
        {
            _texturePath = texturePath;
            _texture = iTextureResourceFactory::getInstance().loadFile(_texturePath);

            update();
        }
    }

    const iaString& iWidgetButton::getTexture() const
    {
        return _texturePath;
    }

	void iWidgetButton::update()
	{
        if (!_text.isEmpty())
        {
            float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();
            int32 textWidth = iWidgetManager::getInstance().getTheme()->getFont()->measureWidth(_text, fontSize);

            if (textWidth + fontSize * 2 > _width)
            {
                _width = textWidth + fontSize * 2;
            }

            if (fontSize * 1.5 > _height)
            {
                _height = fontSize * 1.5;
            }
        }

		updateParent();
	}

    iHorrizontalAlignment iWidgetButton::getHorrizontalTextAlignment() const
    {
        return _horrizontalTextAlignment;
    }

    iVerticalAlignment iWidgetButton::getVerticalTextAlignment() const
    {
        return _verticalTextAlignment;
    }

    void iWidgetButton::setHorrizontalTextAlignment(iHorrizontalAlignment align)
    {
        _horrizontalTextAlignment = align;
    }

    void iWidgetButton::setVerticalTextAlignment(iVerticalAlignment valign)
    {
        _verticalTextAlignment = valign;
    }

	void iWidgetButton::setWidth(int32 width)
	{
		_width = width;
		update();
	}

	void iWidgetButton::setHeight(int32 height)
	{
		_height = height;
		update();
	}

	void iWidgetButton::draw()
	{
		if (isVisible())
		{
			iWidgetManager::getInstance().getTheme()->drawButton(_posx, _posy, _width, _height, _text, _horrizontalTextAlignment, _verticalTextAlignment, _texture, _widgetAppearanceState, isActive());
		}
	}

}