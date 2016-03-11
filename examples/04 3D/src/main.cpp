#include "Example3D.h"

#include <Igor.h>
using namespace Igor;

int main()
{
    // first thing to do
	Igor::startup();

    // create and run example
	Example3D* example = new Example3D();
	example->run();
	delete example;
	
    // last thing to do
	Igor::shutdown();

	return 0;
}
