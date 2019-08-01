// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iApplication.h>

#include <iWindow.h>
#include <iNodeFactory.h>
#include <iTimer.h>
#include <iPhysics.h>
#include <iProfiler.h>

#include <iaConsole.h>
using namespace IgorAux;

#include <algorithm>


namespace Igor
{

    iApplication::iApplication()
    {
        initStatistics();
    }

    iApplication::~iApplication()
    {
        deinitStatistics();

        if (!_windows.empty())
        {
            con_warn("close windows before shutdown");
        }

        if (_preDrawHandleEvent.hasDelegates())
        {
            con_warn("not all delegates unregistered");
            _preDrawHandleEvent.clear();
        }

        if (_postDrawHandleEvent.hasDelegates())
        {
            con_warn("not all delegates unregistered");
            _postDrawHandleEvent.clear();
        }
    }

    void iApplication::stop()
    {
        _running = false;
    }

    void iApplication::iterate()
    {
        iProfiler::getInstance().nextFrame();
        iProfiler::getInstance().beginSection(_frameSectionID);

        iProfiler::getInstance().beginSection(_handleSectionID);
        iTimer::getInstance().handle();
        iNodeFactory::getInstance().handle();
        iProfiler::getInstance().endSection(_handleSectionID);

        iProfiler::getInstance().beginSection(_physicsSectionID);
        iPhysics::getInstance().handle();
        iProfiler::getInstance().endSection(_physicsSectionID);

        iProfiler::getInstance().beginSection(_userSectionID);
        windowHandle();
        _preDrawHandleEvent();
        iProfiler::getInstance().endSection(_userSectionID);

        iProfiler::getInstance().beginSection(_drawSectionID);
        draw();
        iProfiler::getInstance().endSection(_drawSectionID);

        iProfiler::getInstance().beginSection(_userSectionID);
        _postDrawHandleEvent();
        iProfiler::getInstance().endSection(_userSectionID);

        iProfiler::getInstance().endSection(_frameSectionID);
    }

    void iApplication::run()
    {
        _running = true;

        do
        {
            iterate();
        } while (_running);
    }

    void iApplication::deinitStatistics()
    {
        iProfiler::getInstance().unregisterSection(_frameSectionID);
        iProfiler::getInstance().unregisterSection(_handleSectionID);
        iProfiler::getInstance().unregisterSection(_physicsSectionID);
        iProfiler::getInstance().unregisterSection(_drawSectionID);
        iProfiler::getInstance().unregisterSection(_userSectionID);
    }

    void iApplication::initStatistics()
    {
        _frameSectionID = iProfiler::getInstance().registerSection("app:frame", 0);
        _handleSectionID = iProfiler::getInstance().registerSection("app:handle", 0);
        _physicsSectionID = iProfiler::getInstance().registerSection("app:physics", 0);
        _userSectionID = iProfiler::getInstance().registerSection("app:user", 0);
        _drawSectionID = iProfiler::getInstance().registerSection("app:draw", 0);
    }

    bool iApplication::isRunning()
    {
        return _running;
    }

    void iApplication::draw()
    {      
        for (auto window : _windows)
        {
            if (window->isOpen())
            {
                window->draw();
            }
        }
    }

    void iApplication::windowHandle()
    {      
        for (auto window : _windows)
        {
            if (window->isOpen())
            {
                window->handle();
            }
        }
    }

    void iApplication::addWindow(iWindow* window)
    {
        _windows.push_back(window);
    }

    void iApplication::removeWindow(iWindow* window)
    {
        auto iter = find(_windows.begin(), _windows.end(), window);

        if (iter != _windows.end())
        {
            _windows.erase(iter);
        }
    }

    void iApplication::registerApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate handleDelegate)
    {
        _preDrawHandleEvent.append(handleDelegate);
    }

    void iApplication::unregisterApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate handleDelegate)
    {
        _preDrawHandleEvent.remove(handleDelegate);
    }

    void iApplication::registerApplicationPostDrawHandleDelegate(iApplicationPostDrawHandleDelegate handleDelegate)
    {
        _postDrawHandleEvent.append(handleDelegate);
    }

    void iApplication::unregisterApplicationPostDrawHandleDelegate(iApplicationPostDrawHandleDelegate handleDelegate)
    {
        _postDrawHandleEvent.remove(handleDelegate);
    }

};
