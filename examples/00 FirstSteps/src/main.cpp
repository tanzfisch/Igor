// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include "FirstSteps.h"

#include <Igor.h>
#include <iApplication.h>
using namespace Igor;

#include <iaConsole.h>
using namespace IgorAux;

int main(void)
{
    // call this before you call anything else of Igor
    Igor::startup();

    // create and run first steps class
    FirstSteps* firstSteps = new FirstSteps();
    firstSteps->run();
    delete firstSteps;

    // call this after you are done with using Igor
    Igor::shutdown();

    // as alternative you can also implement your main loop your self like folloed
    // we recommend to use the variant above
        
    // call this before you call anything else of Igor
    Igor::startup();

    for (int i = 0; i < 10; ++i)
    {
        iApplication::getInstance().iterate();
        con_endl("iteration " << i);
    }

    // call this after you are done with using Igor
    Igor::shutdown();

    return 0;
}
