// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iTaskFlushTextures.h>

#include <iTextureResourceFactory.h>
#include <iWindow.h>

namespace Igor
{

    iTaskFlushTextures::iTaskFlushTextures(iWindow* window)
        : iTask(window, iTask::DEFAULT_PRIORITY, true, true)
    {
    }

    void iTaskFlushTextures::run()
    {
        iTextureResourceFactory::getInstance().flush();
        _isRunning = false;
    }

    void iTaskFlushTextures::abort()
    {
        iTextureResourceFactory::getInstance().interruptFlush();

        while (isRunning())
        {
            _sleep(1);
        }

        _isRepeating = false;
    }

};
