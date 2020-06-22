// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "WidgetsExample.h"

#include <igor/igor.h>
using namespace igor;

int main()
{
	igor::startup();

	WidgetsExample *example = new WidgetsExample();
	example->run();
	delete example;

	igor::shutdown();

	return 0;
}
