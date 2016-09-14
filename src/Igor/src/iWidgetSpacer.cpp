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
        setHorrizontalAlignment(iHorrizontalAlignment::Center);
        setVerticalAlignment(iVerticalAlignment::Center);
	}

	void iWidgetSpacer::updateContentSize()
	{
        setContentSize(_configuredWidth, _configuredHeight);
	}

	void iWidgetSpacer::draw(int32 parentPosX, int32 parentPosY)
	{
		// updatePosition(parentPosX, parentPosY);

		if (isVisible())
		{
			iWidgetManager::getInstance().getTheme()->drawSpacer(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight(), getAppearanceState(), isActive());
		}
	}

}