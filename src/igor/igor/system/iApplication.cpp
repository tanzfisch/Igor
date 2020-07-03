// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/system/iApplication.h>

#include <igor/system/iWindow.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/system/iTimer.h>
#include <igor/physics/iPhysics.h>
#include <igor/resources/profiler/iProfiler.h>
#include <igor/evaluation/iEvaluationManager.h>
#include <igor/graphics/iView.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <algorithm>

namespace igor
{

    iApplication::~iApplication()
    {
        if (!_layerStack.getStack().empty())
        {
            con_warn("empty layer stack before shutdown");
            clearLayerStack();
        }

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

    void iApplication::clearLayerStack()
    {
        _layerStack.clear();
    }

    void iApplication::addLayer(iLayer *layer)
    {
        _layerStack.addLayer(layer);
    }

    void iApplication::removeLayer(iLayer *layer)
    {
        _layerStack.removeLayer(layer);
    }

    void iApplication::onEvent(iEvent &event)
    {
        event.dispatch<iWindowCloseEvent_TMP>(IGOR_BIND_EVENT_FUNCTION(iApplication::onWindowClose));

        const auto layers = _layerStack.getStack();
        auto riter = layers.rbegin();
        while (riter != layers.rend())
        {
            (*riter)->onEvent(event);
            if (event.isConsumed())
            {
                break;
            }

            riter++;
        }
    }

    bool iApplication::onWindowClose(iWindowCloseEvent_TMP &event)
    {
        stop();
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
        iNodeManager::getInstance().handle();
        iProfiler::getInstance().endSection(_handleSectionID);

        iProfiler::getInstance().beginSection(_userSectionID);
        windowHandle();
        _preDrawHandleEvent(); // TODO get rid of this

        for (auto layer : _layerStack.getStack())
        {
            layer->onPreDraw();
        }

        iProfiler::getInstance().endSection(_userSectionID);

        iProfiler::getInstance().beginSection(_evaluationSectionID);
        iEvaluationManager::getInstance().handle();
        iProfiler::getInstance().endSection(_evaluationSectionID);

        iProfiler::getInstance().beginSection(_physicsSectionID);
        iPhysics::getInstance().handle();
        iProfiler::getInstance().endSection(_physicsSectionID);

        iProfiler::getInstance().beginSection(_drawSectionID);
        draw();
        iProfiler::getInstance().endSection(_drawSectionID);

        iProfiler::getInstance().beginSection(_userSectionID);
        _postDrawHandleEvent(); // TODO get rid of this

        for (auto layer : _layerStack.getStack())
        {
            layer->onPostDraw();
        }

        iProfiler::getInstance().endSection(_userSectionID);

        iProfiler::getInstance().endSection(_frameSectionID);
    }

    void iApplication::run()
    {
        _running = true;
        initProfiling();

        do
        {
            iterate();
        } while (_running);

        deinitProfiling();
    }

    void iApplication::deinitProfiling()
    {
        iProfiler::getInstance().unregisterSection(_frameSectionID);
        iProfiler::getInstance().unregisterSection(_handleSectionID);
        iProfiler::getInstance().unregisterSection(_evaluationSectionID);
        iProfiler::getInstance().unregisterSection(_physicsSectionID);
        iProfiler::getInstance().unregisterSection(_drawSectionID);
        iProfiler::getInstance().unregisterSection(_userSectionID);
    }

    void iApplication::initProfiling()
    {
        _frameSectionID = iProfiler::getInstance().registerSection("app:frame", 0);
        _handleSectionID = iProfiler::getInstance().registerSection("app:handle", 0);
        _evaluationSectionID = iProfiler::getInstance().registerSection("app:eval", 0);
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

    void iApplication::addWindow(iWindow *window)
    {
        _windows.push_back(window);
    }

    void iApplication::removeWindow(iWindow *window)
    {
        auto iter = find(_windows.begin(), _windows.end(), window);

        if (iter != _windows.end())
        {
            _windows.erase(iter);
        }
    }

    void iApplication::registerApplicationPreDrawHandleDelegate(iPreDrawDelegate handleDelegate)
    {
        _preDrawHandleEvent.append(handleDelegate);
    }

    void iApplication::unregisterApplicationPreDrawHandleDelegate(iPreDrawDelegate handleDelegate)
    {
        _preDrawHandleEvent.remove(handleDelegate);
    }

    void iApplication::registerApplicationPostDrawHandleDelegate(iPostDrawDelegate handleDelegate)
    {
        _postDrawHandleEvent.append(handleDelegate);
    }

    void iApplication::unregisterApplicationPostDrawHandleDelegate(iPostDrawDelegate handleDelegate)
    {
        _postDrawHandleEvent.remove(handleDelegate);
    }

}; // namespace igor
