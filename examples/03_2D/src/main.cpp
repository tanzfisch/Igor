// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "Example2D.h"

#include <igor/igor.h>
using namespace igor;

int main(void)
{
	// first thing to do
	igor::startup();

	// create and run example
	Example2D *example = new Example2D();
	example->run();
	delete example;

	// last thing to do
	igor::shutdown();

	return 0;
}
