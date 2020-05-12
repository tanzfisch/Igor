#include "ExampleCharacterController.h"

#include <Igor.h>
using namespace Igor;

int main()
{
    // first thing to do
	Igor::startup();

    // create and run example
	ExampleCharacterController* example = new ExampleCharacterController();
	example->run();
	delete example;
	
    // last thing to do
	Igor::shutdown();

	return 0;
}
