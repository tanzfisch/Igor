// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "KeyboardExample.h"

#include <igor/igor.h>
using namespace igor;

int main(void)
{
    /*! call this before you call anything else of Igor
    */
    igor::startup();

    // create and run example class
    KeyboardExample *example = new KeyboardExample();
    example->run();
    delete example;

    /*! call this after you are done with using Igor
    */
    igor::shutdown();

    return 0;
}
