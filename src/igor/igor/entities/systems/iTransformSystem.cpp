// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iTransformSystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/iEntity.h>

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

	void iTransformSystem::update(const iaTime &time, iEntityScenePtr scene)
	{
		_traverser.traverse(scene);
	}

} // igor