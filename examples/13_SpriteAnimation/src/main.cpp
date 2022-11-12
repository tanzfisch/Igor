// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "SpriteAnimation.h"

int main(void)
{
	// call this before you call anything else of Igor
	igor::startup();

	// create window and open it
	iWindowPtr window = igor::iApplication::getInstance().createWindow("Igor - Sprite Animation Example");
	window->setClientSize(1024, 768);
	window->setCentered();
	window->open();

	// create example and add it as layer to the application
	igor::iApplication::getInstance().addLayer(new SpriteAnimation(window));
	igor::iApplication::getInstance().addLayer(new iLayerProfiler(window));
	igor::iApplication::getInstance().run();
	iApplication::getInstance().clearLayerStack();

	// destroy window
	igor::iApplication::getInstance().destroyWindow(window);

	// call this after you are done with using Igor
	igor::shutdown();

	return 0;
}
