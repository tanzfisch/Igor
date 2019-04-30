First Steps Example                           {#example00}
===================

In this very first example is explained how to get Igor up and running for the first time. Where to get it and what is minimum needed to start loop a bit and end an application using Igor. 

Build
=====

The code for this example can be found in the folder examples/00 FirstSteps. In a vs[ver] subfolder you will find a visual studio solution which contains example project but also all dependent projects. So you just open this one and than build and start. If everything went right you gonna see the following screen. If not please don't hasitate to contact [tanzfisch](https://github.com/tanzfisch).

![First Steps Example](/images/Example00_Pic1.png)

Main
----

So what is it doing. Let's open main.cpp first.

    #include "FirstSteps.h"

    #include <Igor.h>
    using namespace Igor;

    int main(void)
    {
        /*! call this before you call anything else of Igor
        */
	    Igor::startup();

        // create and run first steps class
        FirstSteps* firstSteps = new FirstSteps();
        firstSteps->run();
	    delete firstSteps;

        /*! call this after you are done with using Igor
        */
        Igor::shutdown();

	    return 0;
    }

Besides including Igor.h you might notice only two Igor related commands are called. Igor::startup and Igor::shutdown.
It is important that all other calls to Igor are made between those calls. Or in other words the very first thing to call from Igor is startup and the very last to call is shutdown.

Igor's startup is not doing much more then initializing the console and reading a configuration file if available but shutdown is doing a lot of cleanup and checking to give you a report e.g. about not release resources.

FirstSteps class
----------------

The class called FirstSteps is what we are going to have a look next. It is just personal flavor to put everything in that class you can also directly start making calls to Igor.

So there is really nothing interesting inside the header file. So we can skip that and have a look at the source file FirstSteps.cpp.

    #include <iApplication.h>
    #include <iTimer.h>
    using namespace Igor;

    // let's use the console to see what happens
    #include <iaConsole.h>
    using namespace IgorAux;

So here we have some more includes. First is iApplication.h. iApplication is handles basically the main loop of Igor once that one is started it will only end if said so.
Second is iTimer.h. That one contains timing functionality. It can tell you the current time since system start or the delta time since the last frame.
And then there is the iaConsole.h which is our logging console. Here you might notice that it's within a different namespace called IgorAux. That's because the most common functionality of Igor was moved to a separate library so one can use it independently from Igor.

Let's have a look at the constructor of the FirstSteps class.

`con_endl("ctor");`

The very first thing is using the console to print some status on the console. `con_endl` is a _normal_ message that has a line break in the end. Check out iConsole.h for more interfaces like con_assert, con_debug, con_err, con_warn and more.

The second line is way more interessting. It's about registering a callback to the main loop of our application.

    iApplication::getInstance().registerApplicationHandleDelegate(ApplicationHandleDelegate(this, &FirstSteps::onHandle));

It creates a delegate with the method `onHandle` of `this` instance and passes it the a register method of our singleton instance of the class iApplication. Internally the delegate will be bound to an event. And when ever this event is triggered the delegate will be called (it's basically something like a signal and slot mechanism).

In the destructor of our class you can see the the handle being unregistered again. This way we keep every thing consistent for a clean shutdown.

Let's have a look at the next method called run.

    void FirstSteps::run()
    {
        con_endl("run");

        // starts the applications endless loop
        iApplication::getInstance().run();
    }

It basically does only one thing. It calls the applications run method which contains the main loop of Igor. `run` will not return until we tell Igor to stop. And we gonna do that in our last method called onHandle.

As said before the onHandle method will be called by Igor every frame. So we just use this to simply let it run a few times controlled by a counter and then make Igor stop by calling `iApplication::getInstance().stop();`.

    void FirstSteps::onHandle()
    {
        // since there is nothing to do for the application besides init and deinit some internal stuff
        // the handle will be called es often as possible. which should be reflected in a verry high frame rate

        // lets count
        _counter++;

        // print the counter
        con_endl("handle " << _counter);
    
        if (_counter >= 5)
        {
            // breaks the applications endless loop
            iApplication::getInstance().stop();
            con_endl("ms since the programm started " << iTimer::getInstance().getTime() - _time);
        }
    }

As alternative to using iApplication::run which starts Igors main loop you can implement your own main loop and call iApplication::iterate in it. You can find an example for that in main.cpp.

And that's already it. This is how we setup Igor, start and stop it again. For further questions please contact [tanzfisch](https://github.com/tanzfisch).