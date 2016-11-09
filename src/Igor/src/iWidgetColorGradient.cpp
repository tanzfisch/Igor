// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetColorGradient.h>

#include <iaConsole.h>
using namespace IgorAux;

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iTextureFont.h>
#include <iTextureResourceFactory.h>
#include <iRenderer.h>

namespace Igor
{

	iWidgetColorGradient::iWidgetColorGradient()
	{
		_configuredWidth = 60;
		_configuredHeight = 20;
        _reactOnMouseWheel = false;

		setHorizontalAlignment(iHorizontalAlignment::Center);
		setVerticalAlignment(iVerticalAlignment::Center);

        _texture = iTextureResourceFactory::getInstance().loadFile("special\\checker.png");
        _gradient.setValue(0.0, iaColor4f(1, 1, 1, 1));
	}

    iWidgetColorGradient::~iWidgetColorGradient()
    {
        _texture = nullptr;
    }

    iWidget* iWidgetColorGradient::createInstance()
    {
        return new iWidgetColorGradient();
    }

	void iWidgetColorGradient::calcMinSize()
	{
        setMinSize(0, 0);
	}

    void iWidgetColorGradient::setGradient(const iGradientColor4f& gradient)
    {
        _gradient = gradient;
    }

    const iGradientColor4f& iWidgetColorGradient::getGradient() const
    {
        return _gradient;
    }

    void iWidgetColorGradient::setUseAlpha(bool useAlpha)
    {
        _useAlpha = useAlpha;
    }

    bool iWidgetColorGradient::isUsingAlpha() const
    {
        return _useAlpha;
    }

	void iWidgetColorGradient::draw()
	{
		if (isVisible())
		{
            iWidgetManager::getInstance().getTheme()->drawTiledRectangle(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight(), _texture);
            iWidgetManager::getInstance().getTheme()->drawGradient(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight(), _gradient);
            iWidgetManager::getInstance().getTheme()->drawRectangle(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight());
		}
	}

}
