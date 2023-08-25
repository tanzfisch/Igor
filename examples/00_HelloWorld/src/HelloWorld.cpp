// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "HelloWorld.h"

HelloWorld::HelloWorld()
    : iLayer(nullptr, "HelloWorldLayer")
{
}

void HelloWorld::onInit()
{
    con_endl("onInit");
}

void HelloWorld::onDeinit()
{
    con_endl("onDeinit");
}

void HelloWorld::onUpdate()
{
    static int counter = 0;

    // great the world
    con_endl("Hello World " << counter++);

    if (counter >= 10)
    {
        // breaks the applications endless loop
        iApplication::getInstance().exit();
    }
}
