// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "VoxelExample.h"

int main()
{
	// call this before you call anything else of Igor
	igor::startup();

	// create window and open it
	iWindowPtr window = igor::iApplication::getInstance().getWindow();
	window->setTitle("Igor - Voxel Example");
	window->setClientSize(1024, 768);
	window->setCentered();
	window->open();

	// create example and add it as layer to the application
	igor::iApplication::getInstance().addLayer(new VoxelExample(window));
	igor::iApplication::getInstance().addLayer(new iLayerProfiler(window));
	igor::iApplication::getInstance().run();

	// call this after you are done with using Igor
	igor::shutdown();

	return 0;
}
