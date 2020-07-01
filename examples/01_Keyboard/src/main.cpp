// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "KeyboardExample.h"

#include <igor/system/iApplication.h>
#include <igor/igor.h>
using namespace igor;

int main(void)
{
    // call this before you call anything else of Igor
    igor::startup();

    // create example and add it as layer to the application
    iApplication::getInstance().addLayer(new KeyboardExample());
    iApplication::getInstance().run();

    // clear layer stack to clean up
    iApplication::getInstance().clearLayerStack();

    // call this after you are done with using Igor
    igor::shutdown();

    return 0;
}
