// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iRenderContextThread.h>

#include <iWindow.h>
#include <iaConsole.h>

namespace Igor
{

    iRenderContextThread::iRenderContextThread(iWindow* window)
	{
        con_assert(nullptr != window, "window is missing");
		_window = window;

        if (!(_renderContext = wglCreateContext(_window->getDeviceContext())))
        {
            _isValid = false;
            con_err_win("can't create render context");
        }
        else
        {
            if (!wglShareLists(_window->getRenderContext(), _renderContext))
            {
                _isValid = false;
                con_err_win("can't share lists");
            }
        }
	}

    bool iRenderContextThread::isValid()
    {
        return _isValid;
    }

	void iRenderContextThread::init()
	{
        if (!wglMakeCurrent(_window->getDeviceContext(), _renderContext))
        {
            _isValid = false;
            con_err_win("can't make render context current");
        }
	}

	void iRenderContextThread::deinit()
	{
		if(!wglMakeCurrent(nullptr, nullptr))
		{
			con_err_win("can't set current to zero");
			return;
		}

		if(!wglDeleteContext(_renderContext))
		{
			con_err_win("can't delete render context");
			return;
		}
	}

};
