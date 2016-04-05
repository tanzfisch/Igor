// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetSpacer.h>

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iTextureFont.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
	iWidgetSpacer::iWidgetSpacer()
		: iWidget(iWidgetType::Spacer)
	{
		_height = 20;
		_width = 60;

        setHorrizontalAlignment(iHorrizontalAlignment::Center);
        setVerticalAlignment(iVerticalAlignment::Center);
	}

	void iWidgetSpacer::update()
	{
		updateParent();
	}

	void iWidgetSpacer::setWidth(int32 width)
	{
		_width = width;
		update();
	}

	void iWidgetSpacer::setHeight(int32 height)
	{
		_height = height;
		update();
	}

	void iWidgetSpacer::draw()
	{
		if (isVisible())
		{
			iWidgetManager::getInstance().getTheme()->drawSpacer(_posx, _posy, _width, _height, _widgetAppearanceState, isActive());
		}
	}

}