#include "CharacterController.h"

#include <iNodeFactory.h>
#include <iNodeTransform.h>
#include <iScene.h>
#include <iPhysics.h>
#include <iPhysicsCollision.h>
#include <iPhysicsJoint.h>
#include <iApplication.h>
using namespace Igor;

CharacterController::CharacterController(iNode* node, int64 materiaID)
{
    // setup character and attache camera to it
    _collision = iPhysics::getInstance().createCylinder(_characterRadius, _characterHeight, iaMatrixd());
    iPhysicsBody* charBody = iPhysics::getInstance().createBody(_collision);
    _bodyID = charBody->getID();
    charBody->setMass(10);
    charBody->registerForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &CharacterController::onApplyForceAndTorque));
    charBody->setMaterial(materiaID);
    charBody->setLinearDamping(0.3);

    iNodeTransform* charTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    charTransform->translate(10,0,0);
    _transformNodeID = charTransform->getID();
    node->insertNode(charTransform);

    iPhysics::getInstance().bindTransformNode(charBody, charTransform);

    iPhysicsJoint* joint = iPhysics::getInstance().createJoint(charBody, nullptr, 4);
    joint->registerSubmitConstraintsDelegate(iSubmitConstraintsDelegate(this, &CharacterController::onSubmitConstraints));

    iApplication::getInstance().registerApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &CharacterController::onHandle));
}

CharacterController::~CharacterController()
{
    iApplication::getInstance().unregisterApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &CharacterController::onHandle));

    iNodeFactory::getInstance().destroyNodeAsync(_transformNodeID);
    iPhysics::getInstance().destroyBody(_bodyID);
iPhysics::getInstance().destroyCollision(_collision);
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

unsigned CharacterController::onRayPreFilter(iPhysicsBody* body, iPhysicsCollision* collision, const void* userData)
{
    if (_bodyID == body->getID())
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void CharacterController::onHandle()
{
    iaMatrixd matrix;
    iaVector3d target;

    iNodeTransform* transform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_transformNodeID));
    transform->getMatrix(matrix);

    target = matrix._pos;
    target._y -= 5;

    vector<ConvexCastReturnInfo> result;
    
    iPhysics::getInstance().convexCast(matrix, target, _collision, iRayPreFilterDelegate(this, &CharacterController::onRayPreFilter), nullptr, result);

    if (result.size())
    {
        iaVector3d diff = matrix._pos - (result[0]._point._vec);

        con_endl(diff);
    }
}
