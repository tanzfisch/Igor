// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetColor.h>

#include <iaConsole.h>
using namespace IgorAux;

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iTextureFont.h>
#include <iTextureResourceFactory.h>

namespace Igor
{

	iWidgetColor::iWidgetColor()
	{
		_configuredWidth = 60;
		_configuredHeight = 20;
        _reactOnMouseWheel = false;

		setHorizontalAlignment(iHorizontalAlignment::Center);
		setVerticalAlignment(iVerticalAlignment::Center);

        _texture = iTextureResourceFactory::getInstance().loadFile("special\\checker.png");
        _color.set(1.0f, 0.0f, 1.0f, 0.5f);
	}

    iWidgetColor::~iWidgetColor()
    {
        _texture = nullptr;
    }

    iWidget* iWidgetColor::createInstance()
    {
        return new iWidgetColor();
    }

    void iWidgetColor::setColor(const iaColor4f& color)
    {
        _color = color;
    }

    const iaColor4f& iWidgetColor::getColor() const
    {
        return _color;
    }

	void iWidgetColor::calcMinSize()
	{
        setMinSize(0, 0);
	}

	void iWidgetColor::draw()
	{
		if (isVisible())
		{
			iWidgetManager::getInstance().getTheme()->drawTiledRectangle(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight(), _texture);

            int32 halfWidth = static_cast<int32>((static_cast<float64>(getActualWidth()) / 2.0) + 0.5);

            iWidgetManager::getInstance().getTheme()->drawFilledRectangle(getActualPosX(), getActualPosY(), halfWidth, getActualHeight(), _color);
            iaColor4f colorNoAlpha = _color;
            colorNoAlpha._a = 1.0f;
            iWidgetManager::getInstance().getTheme()->drawFilledRectangle(getActualPosX() + halfWidth, getActualPosY(), halfWidth, getActualHeight(), colorNoAlpha);

            iWidgetManager::getInstance().getTheme()->drawRectangle(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight());
		}
	}

}