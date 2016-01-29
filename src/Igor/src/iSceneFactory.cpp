// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "iSceneFactory.h"

#include <iScene.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

	iSceneFactory::~iSceneFactory()
	{
		if (sceneCount)
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
        delete scene;
        sceneCount--;

        if (0 > sceneCount)
        {
            con_err("scene count underflow");
        }
    }

}
