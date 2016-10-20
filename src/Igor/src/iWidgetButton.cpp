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
        _reactOnMouseWheel = false;
	}

    iWidgetButton::~iWidgetButton()
    {
        _texture = nullptr;
    }

	void iWidgetButton::setText(const iaString& text)
	{
		_text = text;
		//update();
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
            //update();
        }
    }

    const iaString& iWidgetButton::getTexture() const
    {
        return _texturePath;
    }

	void iWidgetButton::calcMinSize()
	{
		int32 minWidth = 0;
		int32 minHeight = 0;

		if (isGrowingByContent() &&
			!_text.isEmpty())
		{
			float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();
			int32 textWidth = iWidgetManager::getInstance().getTheme()->getFont()->measureWidth(_text, fontSize);

            minWidth = textWidth + fontSize * 2.5;
            minHeight = fontSize * 1.5;
		}

        setMinSize(minWidth, minHeight);
	}

    iHorizontalAlignment iWidgetButton::getHorizontalTextAlignment() const
    {
        return _horizontalTextAlignment;
    }

    iVerticalAlignment iWidgetButton::getVerticalTextAlignment() const
    {
        return _verticalTextAlignment;
    }

    void iWidgetButton::setHorizontalTextAlignment(iHorizontalAlignment align)
    {
        _horizontalTextAlignment = align;
    }

    void iWidgetButton::setVerticalTextAlignment(iVerticalAlignment valign)
    {
        _verticalTextAlignment = valign;
    }

	void iWidgetButton::draw()
	{
		if (isVisible())
		{
			iWidgetManager::getInstance().getTheme()->drawButton(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight(), _text, _horizontalTextAlignment, _verticalTextAlignment, _texture, getAppearanceState(), isActive());
		}
	}

}