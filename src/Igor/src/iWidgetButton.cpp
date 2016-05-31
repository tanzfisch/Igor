// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
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
		_configuredHeight = 10;
		_configuredWidth = 20;
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
		int32 width = _configuredWidth;
		int32 height = _configuredHeight;

		if (isGrowingByContent() &&
			!_text.isEmpty())
		{
			float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();
			int32 textWidth = iWidgetManager::getInstance().getTheme()->getFont()->measureWidth(_text, fontSize);

			if (textWidth + fontSize * 1.5 > width)
			{
				width = textWidth + fontSize * 1.5;
			}

			if (fontSize * 1.5 > height)
			{
				height = fontSize * 1.5;
			}
		}

		iWidget::update(width, height);
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

	void iWidgetButton::draw(int32 parentPosX, int32 parentPosY)
	{
		updatePosition(parentPosX, parentPosY);

		if (isVisible())
		{
			iWidgetManager::getInstance().getTheme()->drawButton(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight(), _text, _horrizontalTextAlignment, _verticalTextAlignment, _texture, getAppearanceState(), isActive());
		}
	}

}