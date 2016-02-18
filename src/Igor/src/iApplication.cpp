// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iApplication.h>

#include <iWindow.h>
#include <iStatistics.h>
#include <iPhysicsManager.h>

#include <iaConsole.h>
using namespace IgorAux;

#include <algorithm>
using namespace std;

namespace Igor
{

    iApplication::iApplication()
    {
    }

    iApplication::~iApplication()
    {
        _windows.flush();

        if (_windows.getList().size())
        {
            con_warn("close windows before shutdown");
        }

        if (_handleEvent.hasDelegates())
        {
            con_warn("not all delegates unregistered");
            _handleEvent.clear();
        }
    }

    void iApplication::stop()
    {
        _running = false;
    }

    void iApplication::run()
    {
        _running = true;

        _frameSectionID = iStatistics::getInstance().registerSection("frame", iaColor4f(1, 1, 1, 1), 0);
        _handleSectionID = iStatistics::getInstance().registerSection("handle", iaColor4f(0, 1, 0, 1), 0);

        do
        {
            iStatistics::getInstance().beginSection(_frameSectionID);
            _windows.flush();

            iStatistics::getInstance().beginSection(_handleSectionID);
            handle();
            iStatistics::getInstance().endSection(_handleSectionID);

            draw();

            iStatistics::getInstance().endSection(_frameSectionID);
        } while (_running);

        iStatistics::getInstance().unregisterSection(_frameSectionID);
        iStatistics::getInstance().unregisterSection(_handleSectionID);
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

    void iApplication::handle()
    {
        _handleEvent();
        iPhysicsManager::getInstance().update();

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

    void iApplication::registerApplicationHandleDelegate(iApplicationHandleDelegate handleDelegate)
    {
        _handleEvent.append(handleDelegate);
    }

    void iApplication::unregisterApplicationHandleDelegate(iApplicationHandleDelegate handleDelegate)
    {
        _handleEvent.remove(handleDelegate);
    }

};
