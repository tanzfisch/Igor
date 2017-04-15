// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iApplication.h>

#include <iWindow.h>
#include <iNodeFactory.h>
#include <iTimer.h>
#include <iPhysics.h>
#include <iStatistics.h>

#include <iaConsole.h>
using namespace IgorAux;

#include <algorithm>
using namespace std;

namespace Igor
{

    iApplication::iApplication()
    {
        initStatistics();
    }

    iApplication::~iApplication()
    {
        deinitStatistics();

        _windows.flush();

        if (_windows.getList().size())
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
        iStatistics::getInstance().nextFrame();
        iStatistics::getInstance().beginSection(_frameSectionID);

        iStatistics::getInstance().beginSection(_handleSectionID);
        iTimer::getInstance().handle();
        _windows.flush();
        iNodeFactory::getInstance().handle();
        iStatistics::getInstance().endSection(_handleSectionID);

        iStatistics::getInstance().beginSection(_physicsSectionID);
        iPhysics::getInstance().handle();
        iStatistics::getInstance().endSection(_physicsSectionID);

        iStatistics::getInstance().beginSection(_userSectionID);
        windowHandle();
        _preDrawHandleEvent();
        iStatistics::getInstance().endSection(_userSectionID);

        iStatistics::getInstance().beginSection(_drawSectionID);
        draw();
        iStatistics::getInstance().endSection(_drawSectionID);

        iStatistics::getInstance().beginSection(_userSectionID);
        _postDrawHandleEvent();
        iStatistics::getInstance().endSection(_userSectionID);

        iStatistics::getInstance().endSection(_frameSectionID);
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
        iStatistics::getInstance().unregisterSection(_frameSectionID);
        iStatistics::getInstance().unregisterSection(_handleSectionID);
        iStatistics::getInstance().unregisterSection(_physicsSectionID);
        iStatistics::getInstance().unregisterSection(_drawSectionID);
        iStatistics::getInstance().unregisterSection(_userSectionID);
    }

    void iApplication::initStatistics()
    {
        _frameSectionID = iStatistics::getInstance().registerSection("app:frame", 0);
        _handleSectionID = iStatistics::getInstance().registerSection("app:handle", 0);
        _physicsSectionID = iStatistics::getInstance().registerSection("app:physics", 0);
        _userSectionID = iStatistics::getInstance().registerSection("app:user", 0);
        _drawSectionID = iStatistics::getInstance().registerSection("app:draw", 0);
    }

    bool iApplication::isRunning()
    {
        return _running;
    }

    void iApplication::draw()
    {      
        for (auto window : _windows.getList())
        {
            if (window->isOpen())
            {
                window->draw();
            }
        }
    }

    void iApplication::windowHandle()
    {      
        for (auto window : _windows.getList())
        {
            if (window->isOpen())
            {
                window->handle();
            }
        }
    }

    void iApplication::addWindow(iWindow* window)
    {
        _windows.add(window);
    }

    void iApplication::removeWindow(iWindow* window)
    {
        _windows.remove(window);
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
