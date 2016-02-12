#include "VoxelExample.h"

#include <Igor.h>
using namespace Igor;

int main()
{
    // the verry first thing to do before using Igor
	Igor::startup();

    // create and run voxel example
	VoxelExample* example = new VoxelExample();
	example->run();
	delete example;
	
    // give Igor a chance to clean up you mess ;-)
	Igor::shutdown();

	return 0;
}
