// Igor game engine
// (c) Copyright 2012-2018 by Martin Loga
// see copyright notice in corresponding header file

#include "iSceneFactory.h"

#include <iScene.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

	iSceneFactory::~iSceneFactory()
	{
		if (sceneCount > 0)
		{
			con_err("possible mem leak. scenes left: " << sceneCount);
		}
	}

    iScene* iSceneFactory::createScene()
    {
        sceneCount++;

        return new iScene();
    }

    void iSceneFactory::destroyScene(iScene* scene)
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

}
