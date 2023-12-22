// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "Example2D.h"

int main(void)
{
	// call this before you call anything else of Igor
	igor::startup();

	// create window and open it
	iWindowPtr window = igor::iApplication::getInstance().getWindow();
	window->setTitle("Igor - 2D Renderer Example");
	window->setClientSize(1024, 768);
	window->setCentered();
	window->open();

	// before we can load any resources make sure the resource dictionary is loaded
    iResourceManager::getInstance().loadResourceDictionary("dictionaries/example_resource_dictionary.xml");

	// create example and add it as layer to the application
	igor::iApplication::getInstance().addLayer(new Example2D(window));
	igor::iApplication::getInstance().addLayer(new iLayerProfiler(window));
	const auto exitCode = igor::iApplication::getInstance().run();

	// call this after you are done with using Igor
	igor::shutdown();

	return exitCode;
}
