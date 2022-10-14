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

    iApplication::iApplication()
    {
        for (auto &value : _blockedEvents)
        {
            value = false;
        }
    }

    iApplication::~iApplication()
    {
        if (!_windows.empty())
        {
            con_warn("close and destroy all windows before shutdown");
        }
    }

    bool iApplication::isBlockedEvent(iEventType eventType)
    {
        return _blockedEvents[(int)eventType];
    }

    void iApplication::unblockEvent(iEventType eventType)
    {
        _blockedEvents[(int)eventType] = false;
    }

    void iApplication::blockEvent(iEventType eventType)
    {
        _blockedEvents[(int)eventType] = true;
    }

    void iApplication::onEvent(iEventPtr event)
    {
        if (_blockedEvents[(int)event->getEventType()])
        {
            return;
        }

        _eventQueueMutex.lock();
        _eventQueue.push_back(event);
        _eventQueueMutex.unlock();
    }

    bool iApplication::dispatchOnStack(iEvent &event, iLayerStack &layerStack)
    {
        const auto layers = layerStack.getStack();
        auto riter = layers.rbegin();
        while (riter != layers.rend())
        {
            (*riter)->onEvent(event);
            if (event.isConsumed())
            {
                return true;
            }

            riter++;
        }

        return false;
    }

    void iApplication::dispatch()
    {
        _eventQueueMutex.lock();
        auto eventQueue = std::move(_eventQueue);
        _eventQueueMutex.unlock();

        for (auto eventPtr : eventQueue)
        {
            iEvent &event = *eventPtr;

            if (event.getEventType() != iEventType::iEventMouseMove &&
                event.getEventType() != iEventType::iEventNodeAddedToScene &&
                event.getEventType() != iEventType::iEventNodeRemovedFromScene)
            {
                con_trace("dispatch event: " << event);
            }

            event.dispatch<iEventWindowClose>(IGOR_BIND_EVENT_FUNCTION(iApplication::onWindowClose));
            dispatchOnStack(event, _layerStack);
        }
    }

    void iApplication::preDraw()
    {
        for (auto layer : _layerStack.getStack())
        {
            layer->onPreDraw();
        }
    }

    bool iApplication::onWindowClose(iEventWindowClose &event)
    {
        bool allClosed = true;

        for (auto window : _windows)
        {
            if (window->isOpen())
            {
                allClosed = false;
                break;
            }
        }

        if (allClosed)
        {
            stop();
        }

        return false;
    }

    void iApplication::stop()
    {
        _running = false;
    }

    void iApplication::iterate()
    {
        iaLogLevel logLevel;
        iProfiler::nextFrame();

        {
            IGOR_PROFILER(app);
            iTimer::getInstance().handle();
            iNodeManager::getInstance().handle();
            windowHandle();
            dispatch();
            preDraw();
        }

        iEvaluationManager::getInstance().handle();
        iPhysics::getInstance().handle();

        draw();
    }

    void iApplication::run()
    {
        _running = true;

        do
        {
            iterate();
        } while (_running);
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

    iWindowPtr iApplication::createWindow()
    {
        iWindowPtr window = new iWindow();
        _windows.push_back(window);
        return window;
    }

    void iApplication::destroyWindow(iWindow *window)
    {
        con_assert(window != nullptr, "zero pointer");
        if (window == nullptr)
        {
            return;
        }

        auto iter = std::find(_windows.begin(), _windows.end(), window);
        if (iter != _windows.end())
        {
            delete window;
            _windows.erase(iter);
        }
        else
        {
            con_err("window with id " << window->getID() << " does not exist");
        }
    }

    iWindow *iApplication::getWindow(iWindowID windowID) const
    {
        auto iter = std::find_if(_windows.begin(), _windows.end(), [windowID](iWindowPtr window)
                                 { return window->getID() == windowID; });

        if (iter != _windows.end())
        {
            return *iter;
        }

        con_err("window with id " << windowID << " does not exist");
        return nullptr;
    }

}; // namespace igor
