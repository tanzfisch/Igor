// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "PhysicsExample.h"

#include <igor/igor.h>
using namespace igor;

int main()
{
	igor::startup();

	PhysicsExample *example = new PhysicsExample();
	example->run();
	delete example;

	igor::shutdown();

	return 0;
}
