// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include "WidgetsLayer.h"

WidgetsLayer::WidgetsLayer(iWindowPtr window)
    : iLayerWidgets(new iWidgetDefaultTheme("StandardFont.png", "WidgetThemePattern.png"), window, "Widgets", 100)
{
}

void WidgetsLayer::onInit()
{
    iLayerWidgets::onInit();

    // update desktop size
    iWidgetManager::getInstance().setDesktopDimensions(getWindow()->getClientWidth(), getWindow()->getClientHeight());
}

void WidgetsLayer::onDeinit()
{
    iLayerWidgets::onDeinit();
}

void WidgetsLayer::onEvent(iEvent &event)
{
    iLayerWidgets::onEvent(event);
}