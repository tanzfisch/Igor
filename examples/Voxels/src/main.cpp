#include "VoxelExample.h"

#include <Igor.h>
using namespace Igor;

int main()
{
	Igor::startup();

	VoxelExample* example = new VoxelExample();
	example->run();
	delete example;
	
	Igor::shutdown();

	return 0;
}
