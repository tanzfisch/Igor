// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetSpacer.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetBaseTheme.h>
#include <igor/resources/texture/iTextureFont.h>

#include <iaux/system/iaConsole.h>
using namespace IgorAux;

namespace Igor
{
	iWidgetSpacer::iWidgetSpacer(const iWidgetPtr parent)
		: iWidget(parent)
	{
		setHorizontalAlignment(iHorizontalAlignment::Center);
		setVerticalAlignment(iVerticalAlignment::Center);
		_reactOnMouseWheel = false;
	}

	iWidgetType iWidgetSpacer::getWidgetType() const
	{
		return iWidgetType::iWidgetSpacer;
	}

	void iWidgetSpacer::calcMinSize()
	{
		setMinSize(0, 0);
	}

	void iWidgetSpacer::draw()
	{
		if (isVisible())
		{
			iWidgetManager::getInstance().getTheme()->drawSpacer(getActualRect(), getState(), isActive());
		}
	}

} // namespace Igor