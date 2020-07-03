// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "MouseExample.h"

int main(void)
{
	// first thing to call before any other igor functionality
	igor::startup();

	// create example and run it
	igor::iApplication::getInstance().addLayer(new MouseExample());
	igor::iApplication::getInstance().run();
	igor::iApplication::getInstance().clearLayerStack();

	// last thing to call to shut down igor propperly
	igor::shutdown();

	return 0;
}
