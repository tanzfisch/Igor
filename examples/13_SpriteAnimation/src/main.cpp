// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "SpriteAnimation.h"

#include <igor/igor.h>
using namespace Igor;

int main(void)
{
    // first thing to do
	Igor::startup();

    // create and run example
	SpriteAnimation*example = new SpriteAnimation();
	example->run();
	delete example;
	
    // last thing to do
	Igor::shutdown();

	return 0;
}
