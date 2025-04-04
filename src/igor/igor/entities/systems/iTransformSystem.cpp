// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iTransformSystem.h>

#include <igor/entities/components/iTransformComponent.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

namespace igor
{

    iTransformSystem::iTransformSystem()
    {
    }

    iEntitySystemStage iTransformSystem::getStage() const
    {
        return iEntitySystemStage::Update;
    }

    void iTransformSystem::onUpdate(const iEntitySceneUpdateContext &context)
    {
        _traverser.traverse(context._scene);
    }

} // igor