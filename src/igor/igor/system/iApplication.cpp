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
        if (!_windows.empty())
        {
            con_warn("close and destroy all windows before shutdown");
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

    void iApplication::onEvent(iEventPtr event)
    {
        _eventQueueMutex.lock();
        _eventQueue.push_back(event);
        _eventQueueMutex.unlock();
    }

    void iApplication::dispatchEventStack()
    {
        _eventQueueMutex.lock();
        auto eventQueue = std::move(_eventQueue);
        _eventQueueMutex.unlock();

        for (auto eventPtr : eventQueue)
        {
            iEvent &event = *eventPtr;

            event.dispatch<iWindowCloseEvent_TMP>(IGOR_BIND_EVENT_FUNCTION(iApplication::onWindowClose));

            iWindow *window = event.getWindow();
            if (window != nullptr)
            {
                iWindowID windowID = window->getID();

                auto iter = _windows.find(windowID);
                if (iter != _windows.end())
                {
                    const auto layers = iter->second._layerStack.getStack();
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
                else
                {
                    con_err("window with id " << windowID << " does not exist");
                }
            }
            else
            {
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
        }
    }

    bool iApplication::onWindowClose(iWindowCloseEvent_TMP &event)
    {
        bool allClosed = true;

        for (auto &pair : _windows)
        {
            if (pair.second._window->isOpen())
            {
                allClosed = false;
                break;
            }
        }

        if (allClosed)
        {
            stop();
        }

        return true;
    }

    void iApplication::stop()
    {
        _running = false;
    }

    void iApplication::iterate()
    {
        iProfiler::getInstance().nextFrame();
        iProfiler::getInstance().beginSection(_frameSectionID);

        iTimer::getInstance().handle();

        iProfiler::getInstance().beginSection(_handleSectionID);
        iNodeManager::getInstance().handle();
        iProfiler::getInstance().endSection(_handleSectionID);

        windowHandle();

        iProfiler::getInstance().beginSection(_userSectionID);

        iProfiler::getInstance().beginSection(_dispatchSectionID);
        dispatchEventStack();
        iProfiler::getInstance().endSection(_dispatchSectionID);

        _preDrawHandleEvent(); // TODO get rid of this

        for (auto &pair : _windows)
        {
            for (auto layer : pair.second._layerStack.getStack())
            {
                layer->onPreDraw();
            }
        }

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

        for (auto &pair : _windows)
        {
            for (auto layer : pair.second._layerStack.getStack())
            {
                layer->onPostDraw();
            }
        }

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
        iProfiler::getInstance().unregisterSection(_dispatchSectionID);
        iProfiler::getInstance().unregisterSection(_evaluationSectionID);
        iProfiler::getInstance().unregisterSection(_physicsSectionID);
        iProfiler::getInstance().unregisterSection(_drawSectionID);
        iProfiler::getInstance().unregisterSection(_userSectionID);
    }

    void iApplication::initProfiling()
    {
        _frameSectionID = iProfiler::getInstance().registerSection("app:frame", 0);
        _handleSectionID = iProfiler::getInstance().registerSection("app:handle", 0);
        _dispatchSectionID = iProfiler::getInstance().registerSection("app:dispatch", 0);
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
        for (auto &pair : _windows)
        {
            if (pair.second._window->isOpen())
            {
                pair.second._window->draw();
            }
        }
    }

    void iApplication::windowHandle()
    {
        for (auto &pair : _windows)
        {
            if (pair.second._window->isOpen())
            {
                pair.second._window->handle();
            }
        }
    }

    void iApplication::clearLayerStack(iWindow *window)
    {
        if (window == nullptr)
        {
            _layerStack.clear();
            return;
        }

        auto iter = _windows.find(window->getID());
        if (iter != _windows.end())
        {
            iter->second._layerStack.clear();
        }
        else
        {
            con_err("window with id " << window->getID() << " does not exist");
        }
    }

    void iApplication::addLayer(iLayer *layer)
    {
        con_assert(layer != nullptr, "zero pointer");

        if (layer->getWindow() == nullptr)
        {
            _layerStack.addLayer(layer);
            return;
        }

        auto iter = _windows.find(layer->getWindow()->getID());
        if (iter != _windows.end())
        {
            iter->second._layerStack.addLayer(layer);
        }
    }

    void iApplication::removeLayer(iLayer *layer)
    {
        con_assert(layer != nullptr, "zero pointer");

        if (layer->getWindow() == nullptr)
        {
            _layerStack.removeLayer(layer);
            return;
        }

        auto iter = _windows.find(layer->getWindow()->getID());
        if (iter != _windows.end())
        {
            iter->second._layerStack.removeLayer(layer);
        }
    }

    iWindow *iApplication::createWindow()
    {
        WindowData windowData;
        windowData._window = new iWindow();
        _windows[windowData._window->getID()] = windowData;

        return windowData._window;
    }

    void iApplication::destroyWindow(iWindow *window)
    {
        clearLayerStack(window);

        con_assert(window != nullptr, "zero pointer");
        if (window == nullptr)
        {
            return;
        }

        auto iter = _windows.find(window->getID());
        if (iter != _windows.end())
        {
            delete iter->second._window;
            _windows.erase(iter);
        }
        else
        {
            con_err("window with id " << window->getID() << " does not exist");
        }
    }

    iWindow *iApplication::getWindow(iWindowID windowID) const
    {
        auto iter = _windows.find(windowID);
        if (iter != _windows.end())
        {
            return iter->second._window;
        }

        con_err("window with id " << windowID << " does not exist");
        return nullptr;
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
