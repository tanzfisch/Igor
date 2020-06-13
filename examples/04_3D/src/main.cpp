#include "Example3D.h"

#include <igor/igor.h>
using namespace igor;

int main()
{
    // first thing to do
	igor::startup();

    // create and run example
	Example3D* example = new Example3D();
	example->run();
	delete example;
	
    // last thing to do
	igor::shutdown();

	return 0;
}
