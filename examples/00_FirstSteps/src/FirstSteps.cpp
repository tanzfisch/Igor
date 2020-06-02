// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "FirstSteps.h"

#include <igor/os/iApplication.h>
#include <igor/os/iTimer.h>
using namespace Igor;

#include <iaux/system/iaConsole.h>
using namespace IgorAux;

FirstSteps::FirstSteps()
{
    // mark class generation
    con_endl("ctor");

    // registers our handle to the application handle event. it will be called every frame
    iApplication::getInstance().registerApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &FirstSteps::onHandle));
}

FirstSteps::~FirstSteps()
{
    // unregister our handle again for a clean shutdown
    iApplication::getInstance().unregisterApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &FirstSteps::onHandle));

    // mark class destruction
    con_endl("dtor");
}

void FirstSteps::run()
{
    // mark the application start
    con_endl("run");

    // starts the applications endless loop
    iApplication::getInstance().run();
}

void FirstSteps::onHandle()
{
    // since there is nothing to do for the application besides init and deinit some internal stuff
    // the handle will be called es often as possible. which should be reflected in a very high frame rate

    // lets count
    _counter++;

    // print the counter
    con_endl("handle " << _counter);

    if (_counter >= 5)
    {
        // breaks the applications endless loop
        iApplication::getInstance().stop();

        // print some information about the time
        con_endl("ms since the programm started " << iTimer::getInstance().getApplicationTime());
        con_endl("the frame rate was " << iTimer::getInstance().getFPS());
    }
}
