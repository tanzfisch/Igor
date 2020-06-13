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
    FirstSteps *firstSteps = new FirstSteps();
    firstSteps->run();
    delete firstSteps;

    // call this after you are done with using Igor
    igor::shutdown();

    // as alternative you can also implement your main loop your self like folloed
    // we recommend to use the variant above

    // call this before you call anything else of Igor
    igor::startup();

    con_info("now the application controls the main loop");

    for (int i = 0; i < 10; ++i)
    {
        iApplication::getInstance().iterate();
        con_endl("iteration " << i + 1);
    }

    // call this after you are done with using Igor
    igor::shutdown();

    return 0;
}
