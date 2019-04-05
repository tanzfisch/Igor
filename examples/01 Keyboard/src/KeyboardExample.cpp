#include "KeyboardExample.h"

#include <iApplication.h>
#include <iKeyboard.h>
using namespace Igor;

#include <iaConsole.h>
using namespace IgorAux;

KeyboardExample::KeyboardExample()
: _outputSwitch(true)
{
	init();
}

KeyboardExample::~KeyboardExample()
{
	deinit();
}

void KeyboardExample::init()
{
    // print some helpfull text
	con_endl("--- Keyboard Example --- ");
	con_endl("press ESC - to exit");
	con_endl("press F1  - to switch output method");
	con_endl("Keep keyboard focus on \"Keyboard Example\" _window.");
    con_endl("");
    con_endl("!!! One Warning about a missing view is supposed to pop up. !!!");
	con_endl("");

    // init window
	_window.setTitle("Keyboard Example");
    // opening the window will result in a warning since we have no view to render to defined. but for this example we don't need anything to render
	_window.open(); 
    // need to know when the window was closed so we can shut down the application
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &KeyboardExample::onCloseWindow));

    // registers a callback to the key pressed event. called when any key was pressed
    iKeyboard::getInstance().registerKeyDownDelegate(iKeyDownDelegate(this, &KeyboardExample::onKeyPressed));
    // registers a callback to the key release event. called when any key was released
	iKeyboard::getInstance().registerKeyUpDelegate(iKeyUpDelegate(this, &KeyboardExample::onKeyReleased));
    // registers a callback to the specific key pressed event. In this case the ESC key. called only when the esc key was pressed
    iKeyboard::getInstance().registerKeyDownDelegate(iKeyDownSpecificDelegate(this, &KeyboardExample::onKeyESCPressed), iKeyCode::ESC);
    // registers a callback to the ASCII input event. called for every ascii input event
	iKeyboard::getInstance().registerKeyASCIIDelegate(iKeyASCIIDelegate(this, &KeyboardExample::onKeyASCIIInput));

    // some more helpfull text
	con_endl(" --- starting with single key message output mode ---");
}

void KeyboardExample::deinit()
{
    // unregisters all callbacks. in principle this is optional because we shut down here anyway but if you want a clean shutdown you should do that
	iKeyboard::getInstance().unregisterKeyASCIIDelegate(iKeyASCIIDelegate(this, &KeyboardExample::onKeyASCIIInput));
	iKeyboard::getInstance().unregisterKeyDownDelegate(iKeyDownDelegate(this, &KeyboardExample::onKeyPressed));
	iKeyboard::getInstance().unregisterKeyUpDelegate(iKeyUpDelegate(this, &KeyboardExample::onKeyReleased));
    iKeyboard::getInstance().unregisterKeyDownDelegate(iKeyDownSpecificDelegate(this, &KeyboardExample::onKeyESCPressed), iKeyCode::ESC);

    // closes the window if it was not closed already
	_window.close();
    // unregisters an other callback
    _window.unregisterWindowCloseDelegate(WindowCloseDelegate(this, &KeyboardExample::onCloseWindow));
}

void KeyboardExample::run()
{
    // makes the game engine run
    // one of the things it does is runing in an endless loop fetching the message queue so we get our keyboard input events
	iApplication::getInstance().run();
}

void KeyboardExample::onKeyASCIIInput(const char c)
{
    // check in which output mode we are
	if (!_outputSwitch)
	{
        // print character to console
		con(c);
	}
}

void KeyboardExample::onKeyPressed(iKeyCode key)
{
    // check in which output mode we are
	if (_outputSwitch)
	{
        // print to console name and value of the key that was pressed
		con_endl("pressed: " << iKeyCodeMap::getInstance().getKeyName(key) << " (" << static_cast<int>(key) << ")");
	}
}

void KeyboardExample::onKeyReleased(iKeyCode key)
{
    // check in which output mode we are
	if (_outputSwitch)
	{
        // print to console name and value of the key that was released
        con_endl("released: " << iKeyCodeMap::getInstance().getKeyName(key) << " (" << static_cast<int>(key) << ")");
	}

    // switch output mode if key F1 was pressed
	if (key == iKeyCode::F1)
	{
		if (_outputSwitch)
		{
			_outputSwitch = false;
			con_endl(" --- switched to ascii code output mode ---");
		}
		else
		{
			_outputSwitch = true;
            con_endl("");
			con_endl(" --- switched to single key message output mode ---");
		}
	}
}

void KeyboardExample::onKeyESCPressed()
{
    // breaks the endless loop of our running application
    iApplication::getInstance().stop();
}

void KeyboardExample::onCloseWindow()
{
	con_endl("windows was closed");
    // stop the application of the window was closed. 
    // because once the window is closed we loose the keyboard input and we have to close the console manually (with e.g. Ctrl+C)
	iApplication::getInstance().stop();
}
