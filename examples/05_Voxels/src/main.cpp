// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "VoxelExample.h"

#include <igor/igor.h>
using namespace igor;

int main()
{
    // the verry first thing to do before using Igor
	igor::startup();

    // create and run voxel example
	VoxelExample* example = new VoxelExample();
	example->run();
	delete example;
	
    // give Igor a chance to clean up you mess ;-)
	igor::shutdown();

	return 0;
}
