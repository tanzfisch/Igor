// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
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
    _window = igor::iApplication::getInstance().getWindow(); 
    _window->setTitle("Supremacy");
    _window->setClientSize(1920, 1080);
    _window->setCentered();
    _window->open();

    // create example and add it as layer to the application
    igor::iApplication::getInstance().addLayer(new GameLayer(_window));
    igor::iApplication::getInstance().addLayer(new iLayerWidgets(iWidgetThemePtr(new iWidgetDefaultTheme("igor/textures/StandardFont.png", "WidgetThemePattern.png")), _window, "Widgets", 100));
    igor::iApplication::getInstance().addLayer(new iLayerProfiler(_window, "Profiler", 1000, iProfilerVerbosity::FPSAndMetrics));
}

void Supremacy::onRun()
{
    igor::iApplication::getInstance().run();
}

void Supremacy::onDeInit()
{
    igor::shutdown();
}
