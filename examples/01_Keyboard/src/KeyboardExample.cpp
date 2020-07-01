// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "KeyboardExample.h"

#include <igor/system/iApplication.h>
#include <igor/system/iKeyboard.h>
using namespace igor;

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <functional>

KeyboardExample::KeyboardExample()
	: ExampleBase(L"Keyboard Input")
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

void KeyboardExample::onDeinit()
{
}

#define IGOR_BIND_EVENT_FUNCTION(fn) std::bind(&fn, this, std::placeholders::_1)

void KeyboardExample::onEvent(iEvent &event)
{
	event.dispatch<iKeyASCIIEvent_TMP>(IGOR_BIND_EVENT_FUNCTION(KeyboardExample::onKeyASCIIInput));
	event.dispatch<iKeyDownEvent_TMP>(IGOR_BIND_EVENT_FUNCTION(KeyboardExample::onKeyDown));
	event.dispatch<iKeyUpEvent_TMP>(IGOR_BIND_EVENT_FUNCTION(KeyboardExample::onKeyUp));

	if (!event.isConsumed())
	{
		ExampleBase::onEvent(event);
	}
}

bool KeyboardExample::onKeyASCIIInput(iKeyASCIIEvent_TMP &event)
{
	if (!_outputSwitch)
	{
		// print single character to console bypassing all the standard interfaces
		iaConsole::getInstance() << LOCK << event.getChar() << flush << UNLOCK;
	}

	return false;
}

bool KeyboardExample::onKeyDown(iKeyDownEvent_TMP &event)
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

bool KeyboardExample::onKeyUp(iKeyUpEvent_TMP &event)
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
