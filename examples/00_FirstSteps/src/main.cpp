// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "FirstSteps.h"

#include <igor/igor.h>
#include <igor/system/iApplication.h>
#include <igor/system/iTimer.h>
using namespace igor;

#include <iaux/system/iaConsole.h>
using namespace iaux;

int main(void)
{
    // call this before you call anything else of Igor
    igor::startup();

    con_info("letting the engine control the main loop");

    // create and run first steps class
    iApplication::getInstance().addLayer(new FirstSteps());
    iApplication::getInstance().run();

    // call this after you are done with using Igor
    igor::shutdown();

    return 0;
}
