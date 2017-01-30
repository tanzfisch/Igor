// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include "SystemInput.h"

#include "ComponentForceAndTorque.h"
#include "ComponentInput.h"
#include "ComponentTransform.h"

#include <iEntityManager.h>
#include <iNodeTransform.h>
#include <iNodeFactory.h>

#include <iaMatrix.h>
using namespace IgorAux;

void SystemInput::init()
{
    iEntityManager* entityManager = getEntityManager();

    componentIDForceAndTorque = entityManager->getComponentID("ForceAndTorque");
    componentIDInput = entityManager->getComponentID("Input");
    componentIDTransform = entityManager->getComponentID("Transform");
}

void SystemInput::handle()
{
    iEntityManager* entityManager = getEntityManager();

    ComponentForceAndTorque* componentForceAndTorque = static_cast<ComponentForceAndTorque*>(entityManager->getComponent(componentIDForceAndTorque));
    ComponentInput* componentInput = static_cast<ComponentInput*>(entityManager->getComponent(componentIDInput));
    ComponentTransform* componentTransform = static_cast<ComponentTransform*>(entityManager->getComponent(componentIDTransform));

    for (auto entityID : _entities)
    {
        ForceAndTorque* forceAndTorque  = static_cast<ForceAndTorque*>(componentForceAndTorque->getData(entityID));
        InputFlags* inputFlags = static_cast<InputFlags*>(componentInput->getData(entityID));
        uint32* transformNodeID = static_cast<uint32*>(componentTransform->getData(entityID));

        float32 speed = 75;

        const float32 offsetIncrease = 0.1;
        iaMatrixd matrix;
        iaVector3d resultingForce;

        iNodeTransform* transformationNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(*transformNodeID));
        if (transformationNode != nullptr)
        {
            transformationNode->getMatrix(matrix);

            if (inputFlags->_forward)
            {
                iaVector3d foreward = matrix._depth;
                foreward.negate();
                foreward.normalize();
                foreward *= speed;
                resultingForce += foreward;
            }

            if (inputFlags->_backward)
            {
                iaVector3d backward = matrix._depth;
                backward.normalize();
                backward *= speed;
                resultingForce += backward;
            }

            if (inputFlags->_left)
            {
                iaVector3d left = matrix._right;
                left.negate();
                left.normalize();
                left *= speed;
                resultingForce += left;
            }

            if (inputFlags->_right)
            {
                iaVector3d right = matrix._right;
                right.normalize();
                right *= speed;
                resultingForce += right;
            }

            if (inputFlags->_up)
            {
                iaVector3d up = matrix._top;
                up.normalize();
                up *= speed;
                resultingForce += up;
            }

            if (inputFlags->_down)
            {
                iaVector3d down = matrix._top;
                down.negate();
                down.normalize();
                down *= speed;
                resultingForce += down;
            }

            if (inputFlags->_rollLeft)
            {
                forceAndTorque->_torque._z = 50.0;
            }
            else if (inputFlags->_rollRight)
            {
                forceAndTorque->_torque._z = -50.0;
            }
            else
            {
                forceAndTorque->_torque._z = 0.0;
            }

            forceAndTorque->_force = resultingForce;
        }
    }
}
