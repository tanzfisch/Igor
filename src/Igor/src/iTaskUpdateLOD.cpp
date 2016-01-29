// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iTaskUpdateLOD.h>

#include <iScene.h>

namespace Igor
{

    iTaskUpdateLOD::iTaskUpdateLOD(iScene* scene);
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
            _sleep(100);
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
            _sleep(1);
        }
    }

    bool iTaskUpdateLOD::isRunning()
    {
        return _running;
    }

};
