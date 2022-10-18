// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include "KeyboardExample.h"

KeyboardExample::KeyboardExample(iWindow *window)
	: ExampleBase(window, L"Keyboard Input")
{
}

void KeyboardExample::onInit()
{
	// print some helpfull text
	con_info("press F1  - to switch output method");
	con_info("Keep keyboard focus on \"Keyboard Example\" _window.");

	// some more helpfull text
	con_info(" --- starting with single key message output mode ---");
}

void KeyboardExample::onEvent(iEvent &event)
{
	// first call example base
	ExampleBase::onEvent(event);

	event.dispatch<iEventKeyASCII>(IGOR_BIND_EVENT_FUNCTION(KeyboardExample::onKeyASCIIInput));
	event.dispatch<iEventKeyDown>(IGOR_BIND_EVENT_FUNCTION(KeyboardExample::onKeyDown));
	event.dispatch<iEventKeyUp>(IGOR_BIND_EVENT_FUNCTION(KeyboardExample::onKeyUp));
}

bool KeyboardExample::onKeyASCIIInput(iEventKeyASCII &event)
{
	if (!_outputSwitch)
	{
		// print single character to console bypassing all the standard interfaces
		iaConsole::getInstance() << LOCK << event.getChar() << flush << UNLOCK;
	}

	return false;
}

bool KeyboardExample::onKeyDown(iEventKeyDown &event)
{
	if (_outputSwitch)
	{
		// print to console name of the key that was pressed
		con_endl("pressed: " << event.getKey());

		return true;
	}
	else
	{
		return false;
	}
}

bool KeyboardExample::onKeyUp(iEventKeyUp &event)
{
	if (_outputSwitch)
	{
		// print to console name of the key that was released
		con_endl("released: " << event.getKey());
	}

	// switch output mode if key F1 was pressed
	if (event.getKey() == iKeyCode::F1)
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

	return true;
}
