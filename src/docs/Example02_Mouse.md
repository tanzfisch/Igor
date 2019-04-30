Mouse Usage Example                                     {#example02}
===================

In this tutorial we have a look at Mouse input.

Build
=====

The code for this example can be found in the folder examples/02 Mouse. In a vs[ver] subfolder you will find a visual studio solution which contains example project but also all dependent projects. So you just open this one and than build and start. If everything went right you gonna see the following screen. If not please don't hesitate to contact [tanzfisch](https://github.com/tanzfisch).

![Mouse Example](/images/Example02_Pic1.png)

MouseExample class
==================

Basically It's almost the same as in the Keyboard Example except that we register to mouse events instead of Keyboard events. So we jump directly to the initialization where we register to some mouse key and mouse move events.

    // register callback to mouse move event. we use the "full" version here to show more information in the console
    // usually you only need the iMouseMoveEvent respectively registerMouseMoveDelegate
	iMouse::getInstance().registerMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &MouseExample::onMouseMovedFull));
    // register callback to mouse key down event. called for any key on the mouse pressed
	iMouse::getInstance().registerMouseKeyDownDelegate(iMouseKeyDownDelegate(this, &MouseExample::onMouseKeyDown));
    // register callback to mouse key up event. called for any key on the mouse released
	iMouse::getInstance().registerMouseKeyUpDelegate(iMouseKeyUpDelegate(this, &MouseExample::onMouseKeyUp));
    // register callback to mosue wheel event. called when mouse wheel was turned 
	iMouse::getInstance().registerMouseWheelDelegate(iMouseWheelDelegate(this, &MouseExample::onMouseWheel));

Deinit works exactly the same just call the unregister version of the callback binding methods.

    // unregister from all io events
	iMouse::getInstance().unregisterMouseWheelDelegate(iMouseWheelDelegate(this, &MouseExample::onMouseWheel));
	iMouse::getInstance().unregisterMouseKeyUpDelegate(iMouseKeyUpDelegate(this, &MouseExample::onMouseKeyUp));
	iMouse::getInstance().unregisterMouseKeyDownDelegate(iMouseKeyDownDelegate(this, &MouseExample::onMouseKeyDown));
	iMouse::getInstance().unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &MouseExample::onMouseMovedFull));

Let's have a look at the more interesting callback implementations. Mouse key up and down events are straight forward. You get a key code (same type is for Keyboard events) of the mouse key pressed or released.

    void MouseExample::onMouseKeyDown(iKeyCode key)
    {
        // prints if a key was pressed to the console
        con_endl("pressed " << iKeyCodeMap::getInstance().getKeyName(key));
    }

    void MouseExample::onMouseKeyUp(iKeyCode key)
    {
        // prints if a key was released to the console
        con_endl("released " << iKeyCodeMap::getInstance().getKeyName(key));
    }

The mouse wheel event gives you the mouse wheel delta as parameter

    void MouseExample::onMouseWheel(int32 d)
    {
        // prints the mouse wheel delta to the console
	    con_endl("wheel delta " << d);
    }

And the mouse moved full event (there is also a version that just returns the current mouse position) gives you current and last position of the mouse and the window that got the windows message.

    void MouseExample::onMouseMovedFull(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* window)
    {
        // prints old and new mouse position to the console
	    con_endl("mouse moved from (" << x1 << ", " << y1 << ") to (" << x2 <<  ", " << y2 << ")");
    }

Like for the Keyboard device key states or mouse position can be retrived any time directly from the iMouse class. See the Interface documentation for that.
And that's all to it. For further questions please contact [tanzfisch](https://github.com/tanzfisch).