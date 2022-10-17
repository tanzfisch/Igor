// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include "KeyboardExample.h"

int main(void)
{
    // call this before you call anything else of Igor
    igor::startup();

    // create window and open it
    iWindow *window = igor::iApplication::getInstance().createWindow();
    window->setClientSize(1024, 768);
    window->setCentered();
    window->open();

    // create example and add it as layer to the application
    igor::iApplication::getInstance().addLayer(new KeyboardExample(window));
    igor::iApplication::getInstance().run();
    iApplication::getInstance().clearLayerStack();

    // destroy window
    igor::iApplication::getInstance().destroyWindow(window);

    // call this after you are done with using Igor
    igor::shutdown();

    return 0;
}
