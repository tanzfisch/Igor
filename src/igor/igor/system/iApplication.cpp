// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/system/iApplication.h>

#include <igor/system/iWindow.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/system/iTimer.h>
#include <igor/physics/iPhysics.h>
#include <igor/resources/profiler/iProfiler.h>
#include <igor/renderer/iView.h>
#include <igor/entities/iEntitySystemModule.h>

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
        if (_window != nullptr)
        {
            con_warn("close and destroy window before shutdown");
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

    void iApplication::onUpdateLayerStack()
    {
        for (auto layer : _layerStack.getStack())
        {
            layer->onUpdate();
        }
    }

    bool iApplication::onWindowClose(iEventWindowClose &event)
    {
        exit();
        return false;
    }

    void iApplication::exit(int32 exitCode)
    {
        _running = false;
        _exitCode = exitCode;
    }

    void iApplication::iterate()
    {
        iTimer::getInstance().nextFrame();
        iProfiler::nextFrame();

        IGOR_PROFILER_BEGIN(application);
        iTimer::getInstance().onUpdate();

        iaEventPool::getInstance().execute();

        updateWindow();
        dispatch();
        onUpdateLayerStack();
        IGOR_PROFILER_END(application);

        IGOR_PROFILER_BEGIN(entities);
        iEntitySystemModule::getInstance().onUpdate();
        IGOR_PROFILER_END(entities);

        IGOR_PROFILER_BEGIN(nodes);
        iNodeManager::getInstance().onUpdate();
        IGOR_PROFILER_END(nodes);

        IGOR_PROFILER_BEGIN(physics);
        iPhysics::getInstance().onUpdate();
        IGOR_PROFILER_END(physics);

        draw();
    }

    void iApplication::pause(bool pause)
    {
        if (_paused == pause)
        {
            return;
        }

        _paused = pause;

        if (_paused)
        {
            iTimer::getInstance().stop();
            iPhysics::getInstance().stop();
        }
        else
        {
            iTimer::getInstance().start();
            iPhysics::getInstance().start();
        }
    }

    bool iApplication::isPaused() const
    {
        return _paused;
    }

    int32 iApplication::run()
    {
        _running = true;

        do
        {
            iterate();
        } while (_running);

        clearLayers();
        destroyWindow();

        return _exitCode;
    }

    bool iApplication::isRunning()
    {
        return _running;
    }

    void iApplication::draw()
    {
        if (_window == nullptr ||
            !_window->isOpen())
        {
            return;
        }

        _window->draw();
    }

    void iApplication::updateWindow()
    {
        if (_window == nullptr ||
            !_window->isOpen())
        {
            return;
        }

        _window->onUpdate();
    }

    void iApplication::clearLayers()
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

    void iApplication::destroyWindow()
    {
        if (_window != nullptr)
        {
            delete _window;
            _window = nullptr;
        }
    }

    iWindowPtr iApplication::getWindow()
    {
        if (_window == nullptr)
        {
            _window = new iWindow();
        }
        return _window;
    }

}; // namespace igor
