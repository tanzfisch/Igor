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
    iaMatrixd rotate;
    rotate.rotate(M_PI * 0.5, iaAxis::Z);
    _collision = iPhysics::getInstance().createCylinder(_characterRadius, _characterRadius, _characterHeight, rotate);
    iPhysicsBody* charBody = iPhysics::getInstance().createBody(_collision);
    _bodyID = charBody->getID();
    charBody->setMass(_mass);
    charBody->registerForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &CharacterController::onApplyForceAndTorque));
    charBody->setMaterial(materiaID);
    charBody->setLinearDamping(0.2);

    iNodeTransform* charTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    charTransform->translate(15, 5, 0);
    _rootTransformNodeID = charTransform->getID();
    node->insertNode(charTransform);

    iNodeTransform* headTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    headTransform->translate(0, _headHeight, 0);
    _headTransformNodeID = headTransform->getID();
    charTransform->insertNode(headTransform);

    iPhysics::getInstance().bindTransformNode(charBody, charTransform);

    iPhysicsJoint* joint = iPhysics::getInstance().createJoint(charBody, nullptr, 4);
    joint->registerSubmitConstraintsDelegate(iSubmitConstraintsDelegate(this, &CharacterController::onSubmitConstraints));

    iApplication::getInstance().registerApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &CharacterController::onHandle));
}

CharacterController::~CharacterController()
{
    iApplication::getInstance().unregisterApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &CharacterController::onHandle));

    iNodeFactory::getInstance().destroyNodeAsync(_rootTransformNodeID);
    iPhysics::getInstance().destroyBody(_bodyID);
    iPhysics::getInstance().destroyCollision(_collision);
}

iNodeTransform* CharacterController::getHeadTransform() const
{
    return static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_headTransformNodeID));
}

iNodeTransform* CharacterController::getRootNode() const
{
    return static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_rootTransformNodeID));
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
    iaVector3d gravityForce;

    iPhysics::getInstance().getMassMatrix(static_cast<void*>(body->getNewtonBody()), mass, Ixx, Iyy, Izz);
    gravityForce.set(0.0f, -mass * static_cast<float64>(__IGOR_GRAVITY__), 0.0f);

    force = gravityForce;
    force += _force;
    force += _correctionForce;

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

    iNodeTransform* transform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_rootTransformNodeID));
    transform->getMatrix(matrix);

    target = matrix._pos;
    target._y -= 5;

    vector<ConvexCastReturnInfo> result;

    iPhysics::getInstance().convexCast(matrix, target, _collision, iRayPreFilterDelegate(this, &CharacterController::onRayPreFilter), nullptr, result);

    _correctionForce.set(0, 0, 0);

    if (result.size())
    {
        iaVector3d diff = matrix._pos - (result[0]._point._vec);
        float64 delta = (diff._y - (_characterHeight * 0.5)) - _stepHeight;

        if (delta < _stepHeight)
        {
            iPhysicsBody* body = iPhysics::getInstance().getBody(_bodyID);
            iaVector3d velocity = body->getVelocity();

            if (velocity._y < 0)
            {
                velocity.negate();
                velocity._x = 0;
                velocity._z = 0;

                _correctionForce += (velocity * _mass / (1.0 / iPhysics::getInstance().getSimulationRate())) * 0.5;
            }

#ifdef DETACH_HEAD
            iNodeTransform* head = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_headTransformNodeID));
#endif
            if (delta < 0)
            {
                //_correctionForce._y += _mass * static_cast<float64>(__IGOR_GRAVITY__);
                _correctionForce._y += (-delta) * _mass * 1000;

#ifdef DETACH_HEAD
                head->setPosition(iaVector3d(0, _headHeight - delta, 0));
#endif
            }
            else
            {
#ifdef DETACH_HEAD
                head->setPosition(iaVector3d(0, _headHeight, 0));
#endif
                _correctionForce._y += (-delta) * _mass * 1000;
            }

            con_endl(delta);
        }

        
    }
}

