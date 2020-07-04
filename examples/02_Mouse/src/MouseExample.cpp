// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include "MouseExample.h"

MouseExample::MouseExample(iWindow *window)
    : ExampleBase(window, L"Mouse Input")
{
}

void MouseExample::onEvent(iEvent &event)
{
    // first call example base
    ExampleBase::onEvent(event);

    event.dispatch<iMouseKeyDownEvent_TMP>(IGOR_BIND_EVENT_FUNCTION(MouseExample::onMouseKeyDownEvent));
    event.dispatch<iMouseKeyUpEvent_TMP>(IGOR_BIND_EVENT_FUNCTION(MouseExample::onMouseKeyUpEvent));
    event.dispatch<iMouseKeyDoubleClickEvent_TMP>(IGOR_BIND_EVENT_FUNCTION(MouseExample::onMouseKeyDoubleClickEvent));
    event.dispatch<iMouseMoveEvent_TMP>(IGOR_BIND_EVENT_FUNCTION(MouseExample::onMouseMoveEvent));
    event.dispatch<iMouseWheelEvent_TMP>(IGOR_BIND_EVENT_FUNCTION(MouseExample::onMouseWheelEvent));
}

bool MouseExample::onMouseKeyDownEvent(iMouseKeyDownEvent_TMP &event)
{
    // prints if a key was pressed to the console
    con_endl("mouse key down " << event.getKey());
    return true;
}

bool MouseExample::onMouseKeyUpEvent(iMouseKeyUpEvent_TMP &event)
{
    // prints if a key was released to the console
    con_endl("mouse key up " << event.getKey());
}

bool MouseExample::onMouseKeyDoubleClickEvent(iMouseKeyDoubleClickEvent_TMP &event)
{
    // prints if a double click was performed
    con_endl("mouse key double clicked " << event.getKey());
    return true;
}

bool MouseExample::onMouseMoveEvent(iMouseMoveEvent_TMP &event)
{
    // prints old and new mouse position to the console
    con_endl("mouse moved from " << event.getLastPosition() << " to " << event.getPosition());
    return true;
}

bool MouseExample::onMouseWheelEvent(iMouseWheelEvent_TMP &event)
{
    // prints the mouse wheel delta to the console
    con_endl("mouse wheel delta " << event.getWheelDelta());
}
