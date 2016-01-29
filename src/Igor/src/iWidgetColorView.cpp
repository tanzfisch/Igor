// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
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
		setHorrizontalAlignment(iHorrizontalAlignment::Center);
		setVerticalAlignment(iVerticalAlignment::Center);

        _texture = iTextureResourceFactory::getInstance().loadFile("special\\checker.png");
        _color.set(1.0f, 0.0f, 1.0f, 0.5f);
	}

    iWidgetColorView::~iWidgetColorView()
    {
        _texture = nullptr;
    }

    void iWidgetColorView::setWidth(int32 width)
    {
        _width = width;
        update();
    }

    void iWidgetColorView::setHeight(int32 height)
    {
        _height = height;
        update();
    }

    void iWidgetColorView::setColor(const iaColor4f& color)
    {
        _color = color;
    }

    const iaColor4f& iWidgetColorView::getColor() const
    {
        return _color;
    }

	void iWidgetColorView::update()
	{
		updateParent();
	}

	void iWidgetColorView::draw()
	{
		if (isVisible())
		{
			iWidgetManager::getInstance().getTheme()->drawTiledRectangle(_posx, _posy, _width, _height, _texture);

            int32 halfWidth = static_cast<int32>((static_cast<float64>(_width) / 2.0) + 0.5);

            iWidgetManager::getInstance().getTheme()->drawFilledRectangle(_posx, _posy, halfWidth, _height, _color);
            iaColor4f colorNoAlpha = _color;
            colorNoAlpha._a = 1.0f;
            iWidgetManager::getInstance().getTheme()->drawFilledRectangle(_posx + halfWidth, _posy, halfWidth, _height, colorNoAlpha);

            iWidgetManager::getInstance().getTheme()->drawRectangle(_posx, _posy, _width, _height);
		}
	}

}
