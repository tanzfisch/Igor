// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include "Supremacy.h"

#include "GameLayer.h"

Supremacy::Supremacy()
{
    onInit();
}

Supremacy::~Supremacy()
{
    onDeInit();
}

void Supremacy::run()
{
    onRun();
}

void Supremacy::onInit()
{
    // call this before you call anything else of Igor
    igor::startup();

    // create window and open it
    _window = igor::iApplication::getInstance().createWindow("GameLayer");
    _window->setClientSize(1920 * 0.5, 1080 * 0.5);
    _window->setCentered();
    _window->open();

    // create example and add it as layer to the application
    igor::iApplication::getInstance().addLayer(new GameLayer(_window));
    igor::iApplication::getInstance().addLayer(new iLayerWidgets(iWidgetThemePtr(new iWidgetDefaultTheme("StandardFont.png", "WidgetThemePattern.png")), _window, "Widgets", 100));
    igor::iApplication::getInstance().addLayer(new iLayerProfiler(_window, "Profiler", 1000, iProfilerVerbosity::FPSAndMetrics));
}

void Supremacy::onRun()
{
    igor::iApplication::getInstance().run();
}

void Supremacy::onDeInit()
{
    iApplication::getInstance().clearLayerStack();

    // destroy window
    igor::iApplication::getInstance().destroyWindow(_window);
    // call this after you are done with using Igor
    igor::shutdown();
}
