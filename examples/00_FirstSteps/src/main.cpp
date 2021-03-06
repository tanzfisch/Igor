// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "FirstSteps.h"

int main(void)
{
    // call this before you call anything else of Igor
    igor::startup();

    // we are not creating a window here just to show that we don't need one

    // create example and add it to the layer stack
    igor::iApplication::getInstance().addLayer(new FirstSteps());

    // run example
    igor::iApplication::getInstance().run();

    // delete example by clearing the layer stack
    igor::iApplication::getInstance().clearLayerStack();

    // call this after you are done with using Igor
    igor::shutdown();

    return 0;
}
