// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "MouseExample.h"

MouseExample::MouseExample(iWindowPtr window)
    : ExampleBase(window, L"Mouse Input")
{
}

void MouseExample::onEvent(iEvent &event)
{
    // first call example base
    ExampleBase::onEvent(event);

    event.dispatch<iEventMouseKeyDown>(IGOR_BIND_EVENT_FUNCTION(MouseExample::onMouseKeyDownEvent));
    event.dispatch<iEventMouseKeyUp>(IGOR_BIND_EVENT_FUNCTION(MouseExample::onMouseKeyUpEvent));
    event.dispatch<iEventMouseKeyDoubleClick>(IGOR_BIND_EVENT_FUNCTION(MouseExample::onMouseKeyDoubleClickEvent));
    event.dispatch<iEventMouseMove>(IGOR_BIND_EVENT_FUNCTION(MouseExample::onMouseMoveEvent));
    event.dispatch<iEventMouseWheel>(IGOR_BIND_EVENT_FUNCTION(MouseExample::onMouseWheelEvent));
}

bool MouseExample::onMouseKeyDownEvent(iEventMouseKeyDown &event)
{
    // prints if a key was pressed to the console
    con_endl("mouse key down " << event.getKey());
    return true;
}

bool MouseExample::onMouseKeyUpEvent(iEventMouseKeyUp &event)
{
    // prints if a key was released to the console
    con_endl("mouse key up " << event.getKey());
    return true;
}

bool MouseExample::onMouseKeyDoubleClickEvent(iEventMouseKeyDoubleClick &event)
{
    // prints if a double click was performed
    con_endl("mouse key double clicked " << event.getKey());
    return true;
}

bool MouseExample::onMouseMoveEvent(iEventMouseMove &event)
{
    // prints old and new mouse position to the console
    con_endl("mouse moved from " << event.getLastPosition() << " to " << event.getPosition());
    return true;
}

bool MouseExample::onMouseWheelEvent(iEventMouseWheel &event)
{
    // prints the mouse wheel delta to the console
    con_endl("mouse wheel delta " << event.getWheelDelta());
    return true;
}
