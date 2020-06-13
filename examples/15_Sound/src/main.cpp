#include "ExampleSound.h"

#include <igor/igor.h>
using namespace igor;

int main()
{
	// first thing to do
	igor::startup();

	// create and run example
	ExampleSound *example = new ExampleSound();
	example->run();
	delete example;

	// last thing to do
	igor::shutdown();

	return 0;
}
