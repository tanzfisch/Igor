// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetColorView.h>

#include <iaConsole.h>
using namespace IgorAux;

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iTextureFont.h>
#include <iTextureResourceFactory.h>

namespace Igor
{

	iWidgetColorView::iWidgetColorView()
		: iWidget(iWidgetType::Label)
	{
		_configuredWidth = 60;
		_configuredHeight = 20;

		setHorrizontalAlignment(iHorrizontalAlignment::Center);
		setVerticalAlignment(iVerticalAlignment::Center);

        _texture = iTextureResourceFactory::getInstance().loadFile("special\\checker.png");
        _color.set(1.0f, 0.0f, 1.0f, 0.5f);
	}

    iWidgetColorView::~iWidgetColorView()
    {
        _texture = nullptr;
    }

    void iWidgetColorView::setColor(const iaColor4f& color)
    {
        _color = color;
    }

    const iaColor4f& iWidgetColorView::getColor() const
    {
        return _color;
    }

	void iWidgetColorView::calcMinSize()
	{
        setMinSize(0, 0);
	}

	void iWidgetColorView::draw(int32 parentPosX, int32 parentPosY)
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
