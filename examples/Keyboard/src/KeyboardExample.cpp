#include "KeyboardExample.h"

#include <Igor.h>
#include <iApplication.h>
using namespace Igor;

#include <iaConsole.h>
using namespace IgorAux;

KeyboardExample::KeyboardExample()
: outputSwitch(true)
{
	init();
}

KeyboardExample::~KeyboardExample()
{
	deinit();
}

void KeyboardExample::init()
{
	con_endl("--- Keyboard Example --- ");
	con_endl("press ESC - to exit");
	con_endl("press F1  - to switch output method");
	con_endl("Keep keyboard focus on \"Keyboard Example\" window.");
	con_endl("");

	window.setTitle("Keyboard Example");
	window.addView(&view); // we don't need a view to just use the keyboard. this just to prevents a warning at startup
	window.open();
	window.registerWindowCloseDelegate(WindowCloseDelegate(this, &KeyboardExample::closeWindow));

	iKeyboard::getInstance().registerKeyDownDelegate(KeyDownDelegateExt(this, &KeyboardExample::keyPressed));
	iKeyboard::getInstance().registerKeyUpDelegate(KeyUpDelegateExt(this, &KeyboardExample::keyReleased));
	iKeyboard::getInstance().registerKeyASCIIDelegate(KeyASCIIDelegate(this, &KeyboardExample::keyASCIIInput));

	con_endl("switched to single key message output mode");
}

void KeyboardExample::deinit()
{
	iKeyboard::getInstance().unregisterKeyASCIIDelegate(KeyASCIIDelegate(this, &KeyboardExample::keyASCIIInput));
	iKeyboard::getInstance().unregisterKeyDownDelegate(KeyDownDelegateExt(this, &KeyboardExample::keyPressed));
	iKeyboard::getInstance().unregisterKeyUpDelegate(KeyUpDelegateExt(this, &KeyboardExample::keyReleased));

	window.close();
	window.removeView(&view);
}

void KeyboardExample::run()
{
	iApplication::getInstance().run();
}

void KeyboardExample::keyASCIIInput(uint8 c)
{
	if (!outputSwitch)
	{
		con(c);
	}
}

void KeyboardExample::keyPressed(iKeyCode key)
{
	if (outputSwitch)
	{
		con_endl("\rpressed: " << iKeyCodeMap::getInstance().getKeyName(key));
	}
}

void KeyboardExample::keyReleased(iKeyCode key)
{
	if (outputSwitch)
	{
		con_endl("\rreleased: " << iKeyCodeMap::getInstance().getKeyName(key));
	}

	if (key == iKeyCode::ESC)
	{
		iApplication::getInstance().stop();
	}

	if (key == iKeyCode::F1)
	{
		if (outputSwitch)
		{
			outputSwitch = false;
			con_endl("switched to ascii code output mode");
		}
		else
		{
			outputSwitch = true;
			con_endl("switched to single key message output mode");
		}
	}
}

void KeyboardExample::closeWindow()
{
	con_endl("windows was closed");
	iApplication::getInstance().stop();
}
