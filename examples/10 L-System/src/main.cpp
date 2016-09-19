#include "LSystems.h"

#include <Igor.h>
using namespace Igor;

int main()
{
    // first thing to do
	Igor::startup();

    // create and run example
    LSystems* example = new LSystems();
	example->run();
	delete example;
	
    // last thing to do
	Igor::shutdown();

	return 0;
}
