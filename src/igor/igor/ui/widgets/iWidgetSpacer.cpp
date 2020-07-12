// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetSpacer.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/resources/texture/iTextureFont.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{
	iWidgetSpacer::iWidgetSpacer(int32 width, int32 height, bool visible, const iWidgetPtr parent)
		: iWidget(iWidgetType::iWidgetSpacer, iWidgetKind::Widget, parent)
	{
		setHorizontalAlignment(iHorizontalAlignment::Center);
		setVerticalAlignment(iVerticalAlignment::Center);
		_reactOnMouseWheel = false;

		setWidth(width);
		setHeight(height);
		setVisible(visible);
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

} // namespace igor