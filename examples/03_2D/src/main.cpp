// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "Example2D.h"

int main(void)
{
	// call this before you call anything else of Igor
	igor::startup();

	// create window an let the layer example code control it
	iWindow *window = igor::iApplication::getInstance().createWindow();

	// create example and add it as layer to the application
	igor::iApplication::getInstance().addLayer(new Example2D(window));
	igor::iApplication::getInstance().addLayer(new iLayerProfiler(window));
	igor::iApplication::getInstance().run();

	// destroying a window also destroys all related layers
	igor::iApplication::getInstance().destroyWindow(window);

	// call this after you are done with using Igor
	igor::shutdown();

	return 0;
}
