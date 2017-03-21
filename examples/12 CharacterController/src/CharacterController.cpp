#include "CharacterController.h"

#include <iNodeFactory.h>
#include <iNodeTransform.h>
#include <iScene.h>
#include <iPhysics.h>
#include <iPhysicsCollision.h>
#include <iPhysicsJoint.h>
using namespace Igor;

CharacterController::CharacterController(iNode* node, int64 materiaID)
{
    // setup character and attache camera to it
    iPhysicsCollision* charCollision = iPhysics::getInstance().createCylinder(0.4, 1, iaMatrixd());
    iPhysicsBody* charBody = iPhysics::getInstance().createBody(charCollision);
    charBody->setMass(10);
    charBody->registerForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &CharacterController::onApplyForceAndTorque));
    charBody->setMaterial(materiaID);
    charBody->setLinearDamping(0.3);

    iNodeTransform* charTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _transformNodeID = charTransform->getID();
    node->insertNode(charTransform);

    iPhysics::getInstance().bindTransformNode(charBody, charTransform);

    iPhysicsJoint* joint = iPhysics::getInstance().createJoint(charBody, nullptr, 4);
    joint->registerSubmitConstraintsDelegate(iSubmitConstraintsDelegate(this, &CharacterController::onSubmitConstraints));
}

CharacterController::~CharacterController()
{
}

iNode* CharacterController::getRootNode() const
{
    return iNodeFactory::getInstance().getNode(_transformNodeID);
}

void CharacterController::setForce(const iaVector3d& force)
{
    _force = force;
}

iaVector3d CharacterController::getForce() const
{
    return _force;
}

void CharacterController::onSubmitConstraints(iPhysicsJoint* joint, float32 timestep)
{
    iPhysicsBody* body0 = iPhysics::getInstance().getBody(joint->getBody0ID());
    iaMatrixd matrixBody0;
    body0->getMatrix(matrixBody0);

    bool frontset = false;
    bool upset = false;

    iaVector3d lateralDir(matrixBody0._right % iaVector3d(1, 0, 0));
    float64 mag = lateralDir * lateralDir;

    if (mag > 1.0e-6f)
    {
        // if the side vector is not zero, it means the body has rotated
        mag = sqrt(mag);
        lateralDir /= mag;
        float64 angle = sin(mag);

        // add an angular constraint to correct the error angle
        iPhysics::getInstance().setUserJointAddAngularRow(joint, angle, lateralDir);

        // in theory only one correction is needed, but this produces instability as the body may move sideway.
        // a lateral correction prevent this from happening.
        iaVector3d frontDir(lateralDir % iaVector3d(1, 0, 0));
        iPhysics::getInstance().setUserJointAddAngularRow(joint, 0.0, frontDir);

        frontset = true;
    }

    lateralDir = matrixBody0._top % iaVector3d(0, 1, 0);
    mag = lateralDir * lateralDir;
    if (mag > 1.0e-6f)
    {
        // if the side vector is not zero, it means the body has rotated
        mag = sqrt(mag);
        lateralDir /= mag;
        float64 angle = sin(mag);

        // add an angular constraint to correct the error angle
        iPhysics::getInstance().setUserJointAddAngularRow(joint, angle, lateralDir);

        // in theory only one correction is needed, but this produces instability as the body may move sideway.
        // a lateral correction prevent this from happening.
        iaVector3d topDir(lateralDir % iaVector3d(0, 1, 0));
        iPhysics::getInstance().setUserJointAddAngularRow(joint, 0.0, topDir);

        upset = true;
    }

    if (frontset && upset)
    {
    }
    else
    {
        if ((!frontset) && (!upset))
        {
            iPhysics::getInstance().setUserJointAddAngularRow(joint, 0.0, iaVector3d(1, 0, 0));
            iPhysics::getInstance().setUserJointAddAngularRow(joint, 0.0, iaVector3d(0, 1, 0));
        }
        iPhysics::getInstance().setUserJointAddAngularRow(joint, 0.0, iaVector3d(0, 0, 1));
    }
}

void CharacterController::onApplyForceAndTorque(iPhysicsBody* body, float32 timestep)
{
    float64 Ixx;
    float64 Iyy;
    float64 Izz;
    float64 mass;
    iaVector3d force;

    iPhysics::getInstance().getMassMatrix(static_cast<void*>(body->getNewtonBody()), mass, Ixx, Iyy, Izz);
    force.set(0.0f, -mass * static_cast<float32>(__IGOR_GRAVITY__), 0.0f);
    force += _force;

    body->setForce(force);
}
