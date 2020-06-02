// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iTaskUpdateLOD.h>

#include <igor/graphics/scene/iScene.h>

#include <thread>

namespace Igor
{

    iTaskUpdateLOD::iTaskUpdateLOD(iScene *scene)
        : iTask(nullptr)
    {
        _needsRenderContext = false;
    }

    void iTaskUpdateLOD::run()
    {
        _running = true;

        while (_running)
        {
            iModelResourceFactory::getInstance().flush();
            iTextureResourceFactory::getInstance().flush();
            std::this_thread::yield();
        }

        _done = true;
    }

    void iTaskUpdateLOD::stop()
    {
        _running = false;

        iTextureResourceFactory::getInstance().interruptFlush();
        iModelResourceFactory::getInstance().interruptFlush();

        while (!_done)
        {
            std::this_thread::yield();
        }
    }

    bool iTaskUpdateLOD::isRunning()
    {
        return _running;
    }

}; // namespace Igor
