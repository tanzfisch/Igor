// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "FirstSteps.h"

#include <igor/system/iTimer.h>
using namespace igor;

#include <iaux/system/iaConsole.h>
using namespace iaux;

FirstSteps::FirstSteps()
    : ExampleBase(nullptr, "First Steps", false)
{
}

void FirstSteps::onInit()
{
    con_endl("initialize");
}

void FirstSteps::onDeinit()
{
    con_endl("deinitialize");
}

void FirstSteps::onPreDraw()
{
    // since there is nothing to do for the application besides init and deinit some internal stuff
    // the handle will be called es often as possible. which should be reflected in a very high frame rate

    // lets count
    _counter++;

    // print the counter
    con_endl("iteration " << _counter);

    if (_counter >= 10)
    {
        // breaks the applications endless loop
        iApplication::getInstance().stop();
    }
}
