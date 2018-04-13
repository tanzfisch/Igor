#include "MouseExample.h"

#include <iaConsole.h>
using namespace IgorAux;

#include <iMouse.h>
#include <iKeyboard.h>
#include <iApplication.h>
using namespace Igor;

MouseExample::MouseExample()
{
    // initialize example
	init();
}

MouseExample::~MouseExample()
{
    // deinitialize example
	deinit();
}

void MouseExample::init()
{
    // print some informative text
	con_endl("--- Mouse Example ---");
	con_endl("press ESC - to exit");
	con_endl("Keep keyboard focus on window \"Mouse Example\"");
	con_endl("Move the mouse over that window, click or use mouse wheel.");
	con_endl("");
    con_endl("!!! One Warning about a missing view is supposed to pop up. !!!");
    con_endl("");
	
    // set title of window and open it
    _window.setTitle("Igor - Mouse Example");
    _window.setDoubleClick(true);
	_window.open();

    // register to close window event so we can shutdown the application correctly
	_window.registerWindowCloseDelegate(WindowCloseDelegate(this, &MouseExample::onCloseWindow));

    // register callback to mouse move event. we use the "full" version here to show more information in the console
    // usually you only need the iMouseMoveEvent respectively registerMouseMoveDelegate
	iMouse::getInstance().registerMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &MouseExample::onMouseMovedFull));
    // register callback to mouse key down event. called for any key on the mouse pressed
	iMouse::getInstance().registerMouseKeyDownDelegate(iMouseKeyDownDelegate(this, &MouseExample::onMouseKeyDown));
    // register callback to mouse key up event. called for any key on the mouse released
	iMouse::getInstance().registerMouseKeyUpDelegate(iMouseKeyUpDelegate(this, &MouseExample::onMouseKeyUp));
    // register callback to mouse double click event. called for any key on the mouse double clicked
    iMouse::getInstance().registerMouseDoubleClickDelegate(iMouseKeyDoubleClickDelegate(this, &MouseExample::onMouseDoubleClick));
    // register callback to mouse click event
    iMouse::getInstance().registerMouseClickDelegate(iMouseClickDelegate(this, &MouseExample::onMouseClick));
    // register callback to mosue wheel event. called when mouse wheel was turned 
	iMouse::getInstance().registerMouseWheelDelegate(iMouseWheelDelegate(this, &MouseExample::onMouseWheel));

    // register callback to the ESC key on the keyboard pressed event
	iKeyboard::getInstance().registerKeyDownDelegate(iKeyDownSpecificDelegate(this, &MouseExample::onKeyESCDown), iKeyCode::ESC);
}

void MouseExample::deinit()
{
    // unregister from all io events
    iMouse::getInstance().unregisterMouseDoubleClickDelegate(iMouseKeyDoubleClickDelegate(this, &MouseExample::onMouseDoubleClick));
    iMouse::getInstance().unregisterMouseClickDelegate(iMouseClickDelegate(this, &MouseExample::onMouseClick));
	iMouse::getInstance().unregisterMouseWheelDelegate(iMouseWheelDelegate(this, &MouseExample::onMouseWheel));
	iMouse::getInstance().unregisterMouseKeyUpDelegate(iMouseKeyUpDelegate(this, &MouseExample::onMouseKeyUp));
	iMouse::getInstance().unregisterMouseKeyDownDelegate(iMouseKeyDownDelegate(this, &MouseExample::onMouseKeyDown));
	iMouse::getInstance().unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &MouseExample::onMouseMovedFull));
	iKeyboard::getInstance().unregisterKeyDownDelegate(iKeyDownSpecificDelegate(this, &MouseExample::onKeyESCDown), iKeyCode::ESC);

    // closes the window if it was not closed allready
	_window.close();

    // unregisters from close event
    _window.unregisterWindowCloseDelegate(WindowCloseDelegate(this, &MouseExample::onCloseWindow));
}

void MouseExample::run()
{
    // run the engine main loop
	iApplication::getInstance().run();
}

void MouseExample::onMouseClick(iKeyCode key)
{
    con_endl("clicked " << iKeyCodeMap::getInstance().getKeyName(key));
}

void MouseExample::onMouseMovedFull(const iaVector2i& from, const iaVector2i& to, iWindow* window)
{
    // prints old and new mouse position to the console
	con_endl("mouse moved from " << from << " to " << to);
}

void MouseExample::onMouseKeyDown(iKeyCode key)
{
    // prints if a key was pressed to the console
	con_endl("pressed " << iKeyCodeMap::getInstance().getKeyName(key));
}

void MouseExample::onMouseDoubleClick(iKeyCode key)
{
    // prints if a double click was performed
    con_endl("double clicked " << iKeyCodeMap::getInstance().getKeyName(key));
}

void MouseExample::onMouseKeyUp(iKeyCode key)
{
    // prints if a key was released to the console
	con_endl("released " << iKeyCodeMap::getInstance().getKeyName(key));
}

void MouseExample::onMouseWheel(int32 d)
{
    // prints the mouse wheel delta to the console
	con_endl("wheel delta " << d);
}

void MouseExample::onKeyESCDown()
{
    // stops application aka engine loop
	iApplication::getInstance().stop();
}

void MouseExample::onCloseWindow()
{
	con_endl("windows was closed");

    // stops application aka engine loop after window was closed. otherwhise you will have to close the console manually
	iApplication::getInstance().stop();
}
