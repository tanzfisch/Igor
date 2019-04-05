// Igor game engine
// (c) Copyright 2012-2018 by Martin Loga
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
    {
        setHorizontalAlignment(iHorizontalAlignment::Center);
        setVerticalAlignment(iVerticalAlignment::Center);
        _reactOnMouseWheel = false;
    }

    iWidget* iWidgetSpacer::createInstance()
    {
        return new iWidgetSpacer();
    }

    void iWidgetSpacer::calcMinSize()
    {
        setMinSize(0, 0);
    }

    void iWidgetSpacer::draw()
    {
        if (isVisible())
        {
            iWidgetManager::getInstance().getTheme()->drawSpacer(getActualRect(), getAppearanceState(), isActive());
        }
    }

}