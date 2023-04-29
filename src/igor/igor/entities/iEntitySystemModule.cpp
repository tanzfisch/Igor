// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntitySystemModule.h>

#include<igor/resources/profiler/iProfiler.h>

namespace igor
{

    class iEntitySceneDeleter
    {
    public:
        void operator()(iEntityScene * p) { delete p; }
    };

    iEntityScenePtr iEntitySystemModule::createScene()
    {
        iEntityScenePtr scene = iEntityScenePtr(new iEntityScene(), iEntitySceneDeleter());
        _scenes.push_back(scene);
        return scene;
    }

    void iEntitySystemModule::onUpdate()
    {
        for (auto scene : _scenes)
        {
            scene->onUpdate();
        }
    }

    void iEntitySystemModule::onRender()
    {
        for (auto scene : _scenes)
        {
            scene->onRender();
        }
    }

} // namespace igor