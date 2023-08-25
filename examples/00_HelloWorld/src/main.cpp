// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "HelloWorld.h"

int main(void)
{
    // call this before you call anything else of Igor
    igor::startup();

    // we are not creating a window here just to show that we don't need one

    // create example and add it to the layer stack
    igor::iApplication::getInstance().addLayer(new HelloWorld());

    // run example
    const auto exitCode = igor::iApplication::getInstance().run();

    // call this after you are done using Igor
    igor::shutdown();

    return exitCode;
}
