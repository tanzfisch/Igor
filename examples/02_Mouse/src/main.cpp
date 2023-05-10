// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "MouseExample.h"

int main(void)
{
	// call this before you call anything else of Igor
	igor::startup();

	// create window and open it
	iWindowPtr window = igor::iApplication::getInstance().getWindow();
	window->setTitle("Igor - Mouse Example");
	window->setClientSize(1024, 768);
	window->setCentered();
	window->setDoubleClick(true);
	window->open();

	// create example and add it as layer to the application
	igor::iApplication::getInstance().addLayer(new MouseExample(window));
	igor::iApplication::getInstance().run();

	// call this after you are done with using Igor
	igor::shutdown();

	return 0;
}
