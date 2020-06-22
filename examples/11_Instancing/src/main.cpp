// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "ExampleInstancing.h"

#include <igor/igor.h>
using namespace Igor;

int main()
{
	// first thing to do
	Igor::startup();

	// create and run example
	ExampleInstancing *example = new ExampleInstancing();
	example->run();
	delete example;

	// last thing to do
	Igor::shutdown();

	return 0;
}
