// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "PhysicsExample.h"

int main()
{
	// first thing to do
	igor::startup();

	// create and run example
	igor::iApplication::getInstance().addLayer(new PhysicsExample());
	igor::iApplication::getInstance().run();
	igor::iApplication::getInstance().clearLayerStack();

	// last thing to do
	igor::shutdown();

	return 0;
}
