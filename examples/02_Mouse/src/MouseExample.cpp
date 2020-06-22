// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include "MouseExample.h"

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <igor/system/iMouse.h>
#include <igor/system/iKeyboard.h>
#include <igor/system/iApplication.h>
using namespace igor;

MouseExample::MouseExample()
    : ExampleBase(L"Mouse Input")
{
}

void MouseExample::onMouseMovedFull(const iaVector2i &from, const iaVector2i &to, iWindow *window)
{
    // prints old and new mouse position to the console
    con_endl("mouse moved from " << from << " to " << to);

    ExampleBase::onMouseMovedFull(from, to, window);
}

void MouseExample::onMouseKeyDown(iKeyCode key)
{
    // prints if a key was pressed to the console
    con_endl("pressed " << key);

    ExampleBase::onMouseKeyDown(key);
}

void MouseExample::onMouseDoubleClick(iKeyCode key)
{
    // prints if a double click was performed
    con_endl("double clicked " << key);

    ExampleBase::onMouseDoubleClick(key);
}

void MouseExample::onMouseKeyUp(iKeyCode key)
{
    // prints if a key was released to the console
    con_endl("released " << key);

    ExampleBase::onMouseKeyUp(key);
}

void MouseExample::onMouseWheel(int32 d)
{
    // prints the mouse wheel delta to the console
    con_endl("wheel delta " << d);

    ExampleBase::onMouseWheel(d);
}
