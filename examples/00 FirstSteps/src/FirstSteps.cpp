#include "FirstSteps.h"

#include <Igor.h>
#include <iApplication.h>
#include <iTimer.h>
using namespace Igor;

// let's use the console to see what happens
#include <iaConsole.h>
using namespace IgorAux;

FirstSteps::FirstSteps()
{
    con_endl("ctor");

    // registers our handle to the application handle event. it will be called every frame
    iApplication::getInstance().registerApplicationHandleDelegate(ApplicationHandleDelegate(this, &FirstSteps::onHandle));

    _time = iTimer::getInstance().getTime();
}

FirstSteps::~FirstSteps()
{
    // unregister our handle again for a clean shutdown
    iApplication::getInstance().unregisterApplicationHandleDelegate(ApplicationHandleDelegate(this, &FirstSteps::onHandle));

    con_endl("dtor");
}

void FirstSteps::run()
{
    con_endl("run");

    // starts the applications endless loop
	iApplication::getInstance().run();
}

void FirstSteps::onHandle()
{
    // since there is nothing to do for the application besides init and deinit some internal stuff
    // the handle will be called es often as possible. which should be reflected in a verry high frame rate

    // lets count
    _counter++;

    // print the counter and frame rate
    con_endl("counter " << _counter << "  fps " << iTimer::getInstance().getFPS());
    
    if (_counter >= 100)
    {
        // breaks the applications endless loop
        iApplication::getInstance().stop();
        con_endl("ms since the programm started " << iTimer::getInstance().getTime() - _time);
    }
}
