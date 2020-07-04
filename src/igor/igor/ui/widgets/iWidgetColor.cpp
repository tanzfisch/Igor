// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetColor.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/resources/texture/iTextureResourceFactory.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

	iWidgetColor::iWidgetColor(const iWidgetPtr parent)
		: iWidget(iWidgetType::iWidgetColor, iWidgetKind::Widget, parent)
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

	void iWidgetColor::setColor(const iaColor4f &color)
	{
		_color = color;
	}

	const iaColor4f &iWidgetColor::getColor() const
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
			iWidgetManager::getInstance().getTheme()->drawTiledRectangle(getActualRect(), _texture);

			int32 halfWidth = static_cast<int32>((static_cast<float64>(getActualWidth()) / 2.0) + 0.5);

			iWidgetManager::getInstance().getTheme()->drawFilledRectangle(iRectanglei(getActualPosX(), getActualPosY(), halfWidth, getActualHeight()), _color);
			iaColor4f colorNoAlpha = _color;
			colorNoAlpha._a = 1.0f;
			iWidgetManager::getInstance().getTheme()->drawFilledRectangle(iRectanglei(getActualPosX() + halfWidth, getActualPosY(), halfWidth, getActualHeight()), colorNoAlpha);

			iWidgetManager::getInstance().getTheme()->drawRectangle(getActualRect());
		}
	}

} // namespace igor
