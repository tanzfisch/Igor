// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "Supremacy.h"

int main(void)
{
	// call this before you call anything else of Igor
	igor::startup();

	// create window and open it
	iWindowPtr window = igor::iApplication::getInstance().createWindow("Supremacy");
	window->setClientSize(1920 * 0.5, 1080 * 0.5);
	window->setCentered();
	window->open();

	// create example and add it as layer to the application
	igor::iApplication::getInstance().addLayer(new Supremacy(window));
	igor::iApplication::getInstance().addLayer(new iLayerProfiler(window, "Profiler", 1000,  iProfilerVerbosity::FPSAndMetrics));
	igor::iApplication::getInstance().run();
	iApplication::getInstance().clearLayerStack();

	// destroy window
	igor::iApplication::getInstance().destroyWindow(window);

	// call this after you are done with using Igor
	igor::shutdown();

	return 0;
}
