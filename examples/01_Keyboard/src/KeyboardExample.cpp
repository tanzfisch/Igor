#include "KeyboardExample.h"

#include <igor/system/iApplication.h>
#include <igor/system/iKeyboard.h>
using namespace igor;

#include <iaux/system/iaConsole.h>
using namespace iaux;

KeyboardExample::KeyboardExample()
	: ExampleBase(L"Keyboard Input")
	, _outputSwitch(true)
{
}

void KeyboardExample::init()
{
	// print some helpfull text
	con_endl("press ESC - to exit");
	con_endl("press F1  - to switch output method");
	con_endl("Keep keyboard focus on \"Keyboard Example\" _window.");
	con_endl("");
	con_endl("!!! One Warning about a missing view is supposed to pop up. !!!");
	con_endl("");

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
}

void KeyboardExample::onKeyASCIIInput(const char c)
{
	// check in which output mode we are
	if (!_outputSwitch)
	{
		// print single character to console bypassing all the standard interfaces
		iaConsole::getInstance() << LOCK << c << UNLOCK;
	}
}

void KeyboardExample::onKeyPressed(iKeyCode key)
{
	// check in which output mode we are
	if (_outputSwitch)
	{
		// print to console name and value of the key that was pressed
		con_endl("pressed: " << key << " (" << static_cast<int>(key) << ")");
	}
}

void KeyboardExample::onKeyReleased(iKeyCode key)
{
	// check in which output mode we are
	if (_outputSwitch)
	{
		// print to console name and value of the key that was released
		con_endl("released: " << key << " (" << static_cast<int>(key) << ")");
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

