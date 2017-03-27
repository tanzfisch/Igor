// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iRenderContextThread.h>

#include <iWindow.h>
#include <iaConsole.h>

namespace Igor
{

    iRenderContextThread::iRenderContextThread(iWindow* window)
	{
        con_assert(nullptr != window, "zero pointer");

		_window = window;

        _renderContext = _window->createRenderContext();
        if (_renderContext != nullptr)
        {
            if (_window->shareLists(_renderContext))
            {
                _isValid = true;
            }
            else
            {
                _isValid = false;
            }
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
        con_assert(nullptr != _renderContext, "zero pointer");

        _window->makeCurrent(nullptr);
        _window->deleteRenderContext(_renderContext);

        _renderContext = nullptr;
        _isValid = false;
	}

};
