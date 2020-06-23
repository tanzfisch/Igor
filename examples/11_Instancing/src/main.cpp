// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "ExampleInstancing.h"

#include <igor/igor.h>

int main()
{
	// first thing to do
	igor::startup();

	// create and run example
	ExampleInstancing *example = new ExampleInstancing();
	example->run();
	delete example;

	// last thing to do
	igor::shutdown();

	return 0;
}
