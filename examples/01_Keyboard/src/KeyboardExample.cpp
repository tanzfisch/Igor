#include "KeyboardExample.h"

#include <igor/system/iApplication.h>
#include <igor/system/iKeyboard.h>
using namespace igor;

#include <iaux/system/iaConsole.h>
using namespace iaux;

KeyboardExample::KeyboardExample()
	: ExampleBase(L"Keyboard Input"), _outputSwitch(true)
{
}

void KeyboardExample::init()
{
	// print some helpfull text
	con_info("press F1  - to switch output method");
	con_info("Keep keyboard focus on \"Keyboard Example\" _window.");

	// registers a callback to the ASCII input event. called for every ascii input event
	iKeyboard::getInstance().registerKeyASCIIDelegate(iKeyASCIIDelegate(this, &KeyboardExample::onKeyASCIIInput));

	// some more helpfull text
	con_info(" --- starting with single key message output mode ---");
}

void KeyboardExample::deinit()
{
	// unregister callback
	iKeyboard::getInstance().unregisterKeyASCIIDelegate(iKeyASCIIDelegate(this, &KeyboardExample::onKeyASCIIInput));
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

	// call base class to inherit the default behaviour
	ExampleBase::onKeyPressed(key);
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
			con_info(" --- switched to ascii code output mode ---");
		}
		else
		{
			_outputSwitch = true;
			con_endl("");
			con_info(" --- switched to single key message output mode ---");
		}
	}

	// call base class to inherit the default behaviour
	ExampleBase::onKeyReleased(key);
}
