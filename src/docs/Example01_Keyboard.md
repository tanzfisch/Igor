Keyboard Usage Example                               {#example01}
======================

In this tutorial we have a look at Keyboard input and how to set up a window.

Build
=====

The code for this example can be found in the folder examples/01 Keyboard. In a vs[ver] subfolder you will find a visual studio solution which contains example project but also all dependent projects. So you just open this one and than build and start. If everything went right you gonna see the following screen. If not please don't hasitate to contact [tanzfisch](https://github.com/tanzfisch).

![Keyboard Example](/images/Example01_Pic1.png)

KeyboardExample class
=====================

Let's have a brief look at the header. So there is two new includes. iWindow because we gonna need a window to receive the keyboard messages and iKeyCodeMap because the callbacks to handle the keyboard messages will receive a type called iKeyCode which is Igor specific key codes.

    #include <iWindow.h>
    #include <iKeyCodeMap.h>
    using namespace Igor;

Next we have a member variable of type iWindow which is the window's instance that we gonna use.

    iWindow _window;

And there are a couple of event handlers related to keyboard and window events.

    void onKeyPressed(iKeyCode key);
    void onKeyReleased(iKeyCode key);
    void onKeyASCIIInput(char c);
    void onKeyESCPressed();
    void onCloseWindow();

Let's have a look at the implementation part now. So there is an other new include this time for the actual Keyboard.

    #include <iKeyboard.h>
    using namespace Igor;

We need that so we can register our callbacks to some key press or release events. Which happens in our init method. Let's have a look at that init method. First we setup and open a window.

    // init window
    _window.setTitle("Keyboard Example");
    // opening the window will result in a warning since we have no view to render to defined. but for this example we don't need anything to render
    _window.open(); 
    // need to know when the window was closed so we can shut down the application
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &KeyboardExample::onCloseWindow));

As you probably noticed there is a warning during runtime. I means that the window was opened even though there was no view attached to it. So it is allowed to have a window without a view but in generall it does not make sence there for this warning reminds you of that when you accidentally forgot to register a view to the window.

And than we register some callbacks to keyboard events.

    // registers a callback to the key pressed event. called when any key was pressed
    iKeyboard::getInstance().registerKeyDownDelegate(iKeyDownDelegate(this, &KeyboardExample::onKeyPressed));
    // registers a callback to the key release event. called when any key was released
    iKeyboard::getInstance().registerKeyUpDelegate(iKeyUpDelegate(this, &KeyboardExample::onKeyReleased));
    // registers a callback to the specific key pressed event. In this case the ESC key. called only when the esc key was pressed
    iKeyboard::getInstance().registerKeyDownDelegate(iKeyDownSpecificDelegate(this, &KeyboardExample::onKeyESCPressed), iKeyCode::ESC);
    // registers a callback to the ASCII input event. called for every ascii input event
    iKeyboard::getInstance().registerKeyASCIIDelegate(iKeyASCIIDelegate(this, &KeyboardExample::onKeyASCIIInput));

So that's it for the initialisation. Let's have a look at the deinit method too.

    // unregisters all callbacks. in principle this is optional because we shut down here anyway but if you want a clean shutdown you should do that
    iKeyboard::getInstance().unregisterKeyASCIIDelegate(iKeyASCIIDelegate(this, &KeyboardExample::onKeyASCIIInput));
    iKeyboard::getInstance().unregisterKeyDownDelegate(iKeyDownDelegate(this, &KeyboardExample::onKeyPressed));
    iKeyboard::getInstance().unregisterKeyUpDelegate(iKeyUpDelegate(this, &KeyboardExample::onKeyReleased));
    iKeyboard::getInstance().unregisterKeyDownDelegate(iKeyDownSpecificDelegate(this, &KeyboardExample::onKeyESCPressed), iKeyCode::ESC);

    // closes the window if it was not closed already
    _window.close();
    // unregisters an other callback
    _window.unregisterWindowCloseDelegate(WindowCloseDelegate(this, &KeyboardExample::onCloseWindow));

Pretty self explanatory. Also in the implementation of our callbacks there is nothing suprising except for onCloseWindow.

    void KeyboardExample::onCloseWindow()
    {
	    con_endl("windows was closed");
        // stop the application of the window was closed. 
        // because once the window is closed we loose the keyboard input and we have to close the console manually (with e.g. Ctrl+C)
	    iApplication::getInstance().stop();
    }

So basically when the window was closed we have to shutdown the application too. Because otherwise the application would continue without having a window and therefore we don't get any more calls to our callbacks. 

In an other scenario when we don't need a window that would be fine or course.

Besides using events the Keyboard device's key states can be retrived any time directly from the iKeyboard class. See the Interface documentation for that.
And that's it. This is how we setup a window and some keyboard event handlers/callbacks/delegates. For further questions please contact [tanzfisch](https://github.com/tanzfisch).
