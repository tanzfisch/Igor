#include "MouseExample.h"

#include <iaConsole.h>
using namespace IgorAux;

#include <Igor.h>
#include <iMouse.h>
#include <iApplication.h>
using namespace Igor;

MouseExample::MouseExample()
{
	init();
}

MouseExample::~MouseExample()
{
	deinit();
}

void MouseExample::init()
{
	con_endl("--- Mouse Example ---");
	con_endl("press ESC - to exit");
	con_endl("Keep keyboard focus on window \"Mouse Example\"");
	con_endl("Move the mouse over that window, click or use mouse wheel.");
	con_endl("");
	
	window.addView(&view); // we don't need a view. this just to prevent a warning at startup
    window.setTitle("Igor - Mouse Example");
	window.open();
	window.registerWindowCloseDelegate(WindowCloseDelegate(this, &MouseExample::closeWindow));

	iMouse::getInstance().registerMouseMoveFullDelegate(MouseMoveFullDelegate(this, &MouseExample::mouseMoved));
	iMouse::getInstance().registerMouseKeyDownDelegate(MouseKeyDownDelegate(this, &MouseExample::mouseKeyDown));
	iMouse::getInstance().registerMouseKeyUpDelegate(MouseKeyUpDelegate(this, &MouseExample::mouseKeyUp));
	iMouse::getInstance().registerMouseWheelDelegate(MouseWheelDelegate(this, &MouseExample::mouseWheel));
	iKeyboard::getInstance().registerKeyDownDelegate(KeyDownDelegate(this, &MouseExample::keyboardESCKeyDown), iKeyCode::ESC);
}

void MouseExample::deinit()
{
	iMouse::getInstance().unregisterMouseWheelDelegate(MouseWheelDelegate(this, &MouseExample::mouseWheel));
	iMouse::getInstance().unregisterMouseKeyUpDelegate(MouseKeyUpDelegate(this, &MouseExample::mouseKeyUp));
	iMouse::getInstance().unregisterMouseKeyDownDelegate(MouseKeyDownDelegate(this, &MouseExample::mouseKeyDown));
	iMouse::getInstance().unregisterMouseMoveFullDelegate(MouseMoveFullDelegate(this, &MouseExample::mouseMoved));
	iKeyboard::getInstance().unregisterKeyDownDelegate(KeyDownDelegate(this, &MouseExample::keyboardESCKeyDown), iKeyCode::ESC);

	window.close();
	window.removeView(&view);
    window.unregisterWindowCloseDelegate(WindowCloseDelegate(this, &MouseExample::closeWindow));
}

void MouseExample::run()
{
	iApplication::getInstance().run();
}

void MouseExample::mouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* window)
{
	con_endl("\r" << x1 << ", " << y1 << ", " << x2 <<  ", " << y2);
}

void MouseExample::mouseKeyDown(iKeyCode key)
{
	con_endl("\rpressed " << iKeyCodeMap::getInstance().getKeyName(key));
}

void MouseExample::mouseKeyUp(iKeyCode key)
{
	con_endl("\rreleased " << iKeyCodeMap::getInstance().getKeyName(key));
}

void MouseExample::mouseWheel(int32 d)
{
	con_endl("\rwheel delta " << d);
}

void MouseExample::keyboardESCKeyDown()
{
	iApplication::getInstance().stop();
}

void MouseExample::closeWindow()
{
	con_endl("windows was closed");
	iApplication::getInstance().stop();
}
