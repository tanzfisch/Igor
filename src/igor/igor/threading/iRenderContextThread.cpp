// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/threading/iRenderContextThread.h>

#include <igor/system/iWindow.h>
#include <iaux/system/iaConsole.h>

namespace igor
{

    iRenderContextThread::iRenderContextThread(iWindow *window)
    {
        con_assert(nullptr != window, "zero pointer");

        _window = window;

        _renderContext = _window->createRenderContext(_window->getDefaultRenderContext());
        if (_renderContext != nullptr)
        {
            _isValid = true;
        }
        else
        {
            _isValid = false;
        }

        con_assert(_isValid, "not valid");
    }

    bool iRenderContextThread::isValid()
    {
        return _isValid;
    }

    void iRenderContextThread::init()
    {
        con_assert(nullptr != _window, "zero pointer");

        if (_window->makeCurrent(_renderContext))
        {
            _isValid = true;
        }
        else
        {
            _isValid = false;
        }

        con_assert(_isValid, "not valid");
    }

    void iRenderContextThread::deinit()
    {
        con_assert(_isValid, "not valid");
        con_assert(nullptr != _window, "zero pointer");
        con_assert(_renderContext != nullptr, "invalid render context");

        _window->makeCurrent(nullptr);
        _window->deleteRenderContext(_renderContext);

        _renderContext = nullptr;
        _isValid = false;
    }

}; // namespace igor
