// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/resources/prefab/iPrefab.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{
    iPrefab::iPrefab(const iParameters &parameters)
        : iResource(parameters)
    {
        _sceneID = parameters.getParameter<iEntitySceneID>(IGOR_RESOURCE_PARAM_ENTITY_SCENE_ID, iEntitySceneID::getInvalid());
    }

    iEntitySceneID iPrefab::getSceneID() const
    {
        return _sceneID;
    }

} // namespace igor