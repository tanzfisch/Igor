// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "Ascent.h"

#include <igor/igor.h>
using namespace igor;

int main()
{
	igor::startup();

	Ascent *ascent = new Ascent();
	ascent->run();
	delete ascent;

	igor::shutdown();

	return 0;
}
