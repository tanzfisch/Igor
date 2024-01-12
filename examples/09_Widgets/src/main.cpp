// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "WidgetsExample.h"
#include "Background3D.h"

int main()
{
	// call this before you call anything else of Igor
	igor::startup();

	iProject::loadProject("project");

	// create window and open it
	iWindowPtr window = iApplication::getInstance().getWindow();
	window->setTitle("Igor - Widgets Example");
	window->setClientSize(1024, 768);
	window->setCentered();
	window->open();

	Background3D* backGround = new Background3D(window);
	iLayerProfilerPtr layerProfiler = new iLayerProfiler(window);

	// create example and add it as layer to the application
	iApplication::getInstance().addLayer(backGround);
	iApplication::getInstance().addLayer(new WidgetsExample(window, layerProfiler, backGround));
	iApplication::getInstance().addLayer(layerProfiler);
	iApplication::getInstance().run();

	// call this after you are done with using Igor
	igor::shutdown();

	return 0;
}
