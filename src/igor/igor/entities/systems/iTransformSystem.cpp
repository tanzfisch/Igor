// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iTransformSystem.h>

#include <igor/entities/components/iTransformComponent.h>
#include <igor/resources/profiler/iProfiler.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

namespace igor
{

    iTransformSystem::iTransformSystem()
    {
        _transformView = createView<iTransformComponent>();
    }

    iEntitySystemStage iTransformSystem::getStage() const
    {
        return iEntitySystemStage::Update;
    }

    void iTransformSystem::onUpdate(const iEntitySceneUpdateContext &context)
    {
        IGOR_PROFILER_SCOPED(transform);
        _traverser.traverse(context._scene);
    }

} // igor