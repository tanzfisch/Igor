// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iPhysicsPlayer.h>
#include <igor/physics/iPhysics.h>
#include <iEntity.h>

namespace Igor
{

    iPhysicsPlayer::iPhysicsPlayer(void *newtonBody)
        : iPhysicsBody(newtonBody)
    {
        _bodyType = PhysicsBodyType::Player;
    }

    void iPhysicsPlayer::ApplyForceAndTorque(float32 timestep, int threadIndex)
    {
        float32 Ixx;
        float32 Iyy;
        float32 Izz;
        float32 mass;

        iPhysics::getInstance().getMassMatrixFromBody(_newtonBody, mass, Ixx, Iyy, Izz);

        iaVector3<float32> force;
        force.set(0.0f, -mass * static_cast<float32>(IGOR_GRAVITY), 0.0f);

        if (nullptr != _entity)
        {
            force += _entity->getForce();

            iaVector3<float32> velocity;
            iPhysics::getInstance().getVelocity(_newtonBody, velocity);
            velocity._y = 0;
            velocity.negate();
            force += velocity / (1.0 / iPhysics::getSimulationRate());
        }

        iPhysics::getInstance().setForce(_newtonBody, force);
    }
} // namespace Igor