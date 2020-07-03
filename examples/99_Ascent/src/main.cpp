// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "Ascent.h"

int main()
{
	igor::startup();

	igor::iApplication::getInstance().addLayer(new Ascent());
	igor::iApplication::getInstance().run();
	igor::iApplication::getInstance().clearLayerStack();

	igor::shutdown();

	return 0;
}
