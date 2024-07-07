// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iAnimationSystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/components/iTransformComponent.h>
#include <igor/system/iTimer.h>

namespace igor
{

    iAnimationSystem::iAnimationSystem()
    {
        _view = createView<iTransformComponent, iAnimationComponent>();
    }

    iEntitySystemStage iAnimationSystem::getStage() const
    {
        return iEntitySystemStage::Update;
    }

    void iAnimationSystem::onUpdate(const iEntitySceneUpdateContext &context)
    {
        const iaTime &time = context._time;

        for (auto entity : _view->getEntities())
        {
            auto animation = entity->getComponent<iAnimationComponent>();            
            animation->_animationController->update(time, entity);
        }
    }

} // igor