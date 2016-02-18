// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iSystemPhysicsUpdate.h>
#include <iNodeFactory.h>
#include <iNodeTransform.h>
#include <iEntityManager.h>
#include <iEntityDataPhysics.h>
#include <iEntityDataTransformation.h>
#include <iPhysicsBody.h>
#include <iPhysics.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
    uint64 iSystemPhysicsUpdate::getDataMask()
    {
        return static_cast<uint64>(iEntityDataMask::Physics);
    }

    void iSystemPhysicsUpdate::handle(uint64 entityID)
    {
        uint64 bodyID = _entityDataPhysics->getBody(entityID);
        iPhysicsBody* body = iPhysics::getInstance().getBody(bodyID);

        if (body != nullptr)
        {
            body->setVelocity(_entityDataPhysics->getVelocity(entityID));

            if (_entityDataPhysics->getMass(entityID) != body->getMass())
            {
                body->setMass(_entityDataPhysics->getMass(entityID));
            }
        }
    }

    void iSystemPhysicsUpdate::onRegistration()
    {
        _entityDataPhysics = static_cast<iEntityDataPhysics*>(iEntityManager::getInstance().getEntityData(static_cast<uint64>(iEntityDataMask::Physics)));

        con_assert(_entityDataPhysics != nullptr, "zero pointer");
    }


}