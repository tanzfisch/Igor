// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include "Supremacy.h"

#include "GameLayer.h"

Supremacy::Supremacy()
{
    // call this before you call anything else of Igor
    igor::startup();    

    // create window and open it
    iWindowPtr window = igor::iApplication::getInstance().getWindow();
    window->setTitle("Supremacy");
    window->setClientSize(1920, 1080);
    window->setCentered();
    window->open();

    iProject::getInstance().load("project/supremacy.project");

    // create example and add it as layer to the application
    igor::iApplication::getInstance().addLayer(new GameLayer(window));
    igor::iApplication::getInstance().addLayer(new iLayerWidgets(iWidgetThemePtr(new iWidgetDefaultTheme(iResourceManager::getInstance().loadResource<iTexture>("igor_font_default"), 
    iResourceManager::getInstance().loadResource<iTexture>("igor_widget_theme_pattern"))), window, "Widgets", 100));
    igor::iApplication::getInstance().addLayer(new iLayerProfiler(window, "Profiler", 1000, iProfilerVerbosity::FPSAndMetrics));
}

Supremacy::~Supremacy()
{
    igor::shutdown();
}

void Supremacy::run()
{
    igor::iApplication::getInstance().run();
}
