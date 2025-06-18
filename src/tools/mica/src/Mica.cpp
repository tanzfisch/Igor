// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "Mica.h"

#include "UILayer.h"

/*! window title definition
*/
static const iaString MICA_TITLE_PREFIX = L"Mica";

Mica::Mica()
{
	_window = iApplication::getInstance().getWindow();
	_window->setTitle(MICA_TITLE_PREFIX);
	_window->setSize(1920, 1080);
	_window->setCentered();
	_window->setDoubleClick(true);
	_window->open();

	iApplication::getInstance().addLayer(new UILayer(_window, 20));
	igor::iApplication::getInstance().addLayer(new iLayerProfiler(_window, "Profiler", 100, iProfilerVerbosity::None));
}

void Mica::run()
{
	iApplication::getInstance().run();
}