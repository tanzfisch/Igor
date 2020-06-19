// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "StateMachineExample.h"

#include <igor/igor.h>
using namespace igor;

int main(void)
{
	igor::startup();

	StateMachineExample *example = new StateMachineExample();
	example->run();
	delete example;

	igor::shutdown();

	return 0;
}
