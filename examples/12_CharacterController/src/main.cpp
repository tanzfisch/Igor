// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "ExampleCharacterController.h"

#include <igor/igor.h>
using namespace igor;

int main()
{
    // first thing to do
	igor::startup();

    // create and run example
	ExampleCharacterController* example = new ExampleCharacterController();
	example->run();
	delete example;
	
    // last thing to do
	igor::shutdown();

	return 0;
}
