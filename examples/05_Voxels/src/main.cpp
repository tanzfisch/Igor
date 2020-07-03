// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "VoxelExample.h"

int main()
{
	// the verry first thing to do before using Igor
	igor::startup();

	// create and run voxel example
	igor::iApplication::getInstance().addLayer(new VoxelExample());
	igor::iApplication::getInstance().run();
	igor::iApplication::getInstance().clearLayerStack();

	// give Igor a chance to clean up you mess ;-)
	igor::shutdown();

	return 0;
}
