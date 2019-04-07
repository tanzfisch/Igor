#include "Example2D.h"

#include <Igor.h>
using namespace Igor;

int main(void)
{
    // first thing to do
	Igor::startup();

    // create and run example
    Example2D *example = new Example2D();
	example->run();
	delete example;
	
    // last thing to do
	Igor::shutdown();

	return 0;
}
