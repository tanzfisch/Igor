// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "iSceneFactory.h"

#include <igor/scene/iScene.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iSceneFactory::~iSceneFactory()
    {
        if (sceneCount > 0)
        {
            con_err("possible mem leak. scenes left: " << sceneCount);
        }
    }

    iScene *iSceneFactory::createScene()
    {
        sceneCount++;

        return new iScene();
    }

    void iSceneFactory::destroyScene(iScene *scene)
    {
        con_assert(scene != nullptr, "zero pointer");

        if (scene != nullptr)
        {
            delete scene;
        }

        sceneCount--;
        if (0 > sceneCount)
        {
            con_err("scene count underflow");
        }
    }

} // namespace igor