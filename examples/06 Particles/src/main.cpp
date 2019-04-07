#include "Particles.h"

#include <Igor.h>
using namespace Igor;

int main()
{
    // first thing to do
	Igor::startup();

    // create and run example
    Particles* example = new Particles();
	example->run();
	delete example;
	
    // last thing to do
	Igor::shutdown();

	return 0;
}
