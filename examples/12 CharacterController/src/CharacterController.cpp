#include "CharacterController.h"

#include <iNodeFactory.h>
#include <iNodeTransform.h>
#include <iScene.h>
#include <iPhysics.h>
#include <iPhysicsCollision.h>
#include <iPhysicsJoint.h>
#include <iApplication.h>
using namespace Igor;

CharacterController::CharacterController(iNodePtr node, int64 materiaID, const iaMatrixd& startMatrix)
{
	// setup character and attache camera to it
	iaMatrixd rotate;
	rotate.rotate(M_PI * 0.5, iaAxis::Z);
	iPhysicsCollision* collision = iPhysics::getInstance().createCylinder(_characterRadius, _characterRadius, _characterHeight, rotate);
	iPhysicsBody* charBody = iPhysics::getInstance().createBody(collision);
	_bodyID = charBody->getID();
	charBody->setMass(_mass);
	charBody->registerForceAndTorqueDelegate(iApplyForceAndTorqueDelegate(this, &CharacterController::onApplyForceAndTorque));
	charBody->setMaterial(materiaID);
	charBody->setLinearDamping(0);
	iPhysics::getInstance().destroyCollision(collision);

	_collisionCast = iPhysics::getInstance().createCylinder(_characterRadius * 0.9, _characterRadius * 0.9, _characterHeight, rotate);

	iNodeTransform* physicsTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	physicsTransform->setMatrix(startMatrix);
	_physicsTransformNodeID = physicsTransform->getID();
	node->insertNode(physicsTransform);

	iNodeTransform* headingTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	_headingTransformNodeID = headingTransform->getID();
	physicsTransform->insertNode(headingTransform);

	iNodeTransform* upperBodyTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	upperBodyTransform->translate(0, 0.55, 0);
	headingTransform->insertNode(upperBodyTransform);

	iNodeTransform* pitchTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	_pitchTransformNodeID = pitchTransform->getID();
	upperBodyTransform->insertNode(pitchTransform);

	iNodeTransform* leftShoulderTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	leftShoulderTransform->translate(-0.2, 0, 0);
	_leftShoulderTransformNodeID = leftShoulderTransform->getID();
	pitchTransform->insertNode(leftShoulderTransform);

	iNodeTransform* rightShoulderTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	rightShoulderTransform->translate(0.2, 0, 0);
	_rightShoulderTransformNodeID = rightShoulderTransform->getID();
	pitchTransform->insertNode(rightShoulderTransform);

	iNodeTransform* headTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	_headTransformNodeID = headTransform->getID();
	headTransform->translate(0, 0.1, 0);
	pitchTransform->insertNode(headTransform);

	iPhysics::getInstance().bindTransformNode(charBody, physicsTransform);

	iPhysicsJoint* joint = iPhysics::getInstance().createJoint(charBody, nullptr, 4);
	joint->registerSubmitConstraintsDelegate(iSubmitConstraintsDelegate(this, &CharacterController::onSubmitConstraints));
}

CharacterController::~CharacterController()
{
	iNodeFactory::getInstance().destroyNodeAsync(_physicsTransformNodeID);
	iPhysics::getInstance().destroyBody(_bodyID);
	iPhysics::getInstance().destroyCollision(_collisionCast);
}

iNodeTransform* CharacterController::getHeadingTransform() const
{
	return static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_headingTransformNodeID));
}

iNodeTransform* CharacterController::getPitchTransform() const
{
	return static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_pitchTransformNodeID));
}

iNodeTransform* CharacterController::getHeadTransform() const
{
	return static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_headTransformNodeID));
}

iNodeTransform* CharacterController::getLeftSholderTransform() const
{
	return static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_leftShoulderTransformNodeID));
}

iNodeTransform* CharacterController::getRightSholderTransform() const
{
	return static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_rightShoulderTransformNodeID));
}

iNodeTransform* CharacterController::getRootNode() const
{
	return static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_physicsTransformNodeID));
}

void CharacterController::setForce(const iaVector3d& force)
{
	_navigationForce = force;
}

iaVector3d CharacterController::getForce() const
{
	return _navigationForce;
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
	iaVector3d correctionForce;
	iterate(correctionForce);

	iaVector3d force;

	// vertical dampening if in contact with floor
	if (_state == CharacterControllerState::Floor)
	{
		iaVector3d verticalVelocity = body->getVelocity();
		verticalVelocity.negate();
		verticalVelocity._x = 0;
		verticalVelocity._z = 0;
		force += (verticalVelocity * _mass / (1.0 / iPhysics::getInstance().getSimulationRate())) * 0.5;
	}
	else
	{
		iaVector3d gravityForce(0.0f, -_mass * static_cast<float64>(__IGOR_GRAVITY__), 0.0f);
		force += gravityForce;
	}

	// horrizontal dampening
	iaVector3d velocity = body->getVelocity();
	velocity.negate();
	velocity._y = 0;
	force += (velocity * _mass / (1.0 / iPhysics::getInstance().getSimulationRate())) * 0.25;

	force += _navigationForce;
	force += correctionForce;

	body->setForce(force);
}

void CharacterController::iterate(iaVector3d & correctionForce)
{
	iaVector3d point;
	iaVector3d normal;
	float64 heightAboveGround = getContactPoint(point, normal);

	float64 delta = heightAboveGround - _stepHeight;
	float64 pullThreashold = _stepHeight;

	switch (_state)
	{
	case CharacterControllerState::Air:
		// prevent double jump
		if (_navigationForce._y > 0)
		{
			_navigationForce._y = 0;
		}

		if (abs(delta) <= pullThreashold)
		{
			_state = CharacterControllerState::Floor;
		}
		break;

	case CharacterControllerState::Floor:
		if (_navigationForce._y <= 0)
		{
			correctionForce._y += (-delta) * _mass * 1000;
		}
		else
		{
			_state = CharacterControllerState::Jumped;
		}
		break;

	case CharacterControllerState::Jumped:
		// prevent double jump
		if (_navigationForce._y > 0)
		{
			_navigationForce._y = 0;
		}

		if (heightAboveGround > 1)
		{
			_state = CharacterControllerState::Air;
		}
		break;
	}

#ifdef DETACH_HEAD
	iNodeTransform* head = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_headTransformNodeID));
	if (delta < 0)
	{
		head->setPosition(iaVector3d(0, _headHeight - delta, 0));
	}
	else
	{
		head->setPosition(iaVector3d(0, _headHeight, 0));
	}
#endif
}


unsigned CharacterController::onRayPreFilter(iPhysicsBody * body, iPhysicsCollision * collision, const void* userData)
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

float64 CharacterController::getContactPoint(iaVector3d & point, iaVector3d & normal)
{
	float64 result = 99999999999;
	iaMatrixd matrix;
	iaVector3d target;

	iNodeTransform* transform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_physicsTransformNodeID));
	transform->getMatrix(matrix);

	target = matrix._pos;
	target._y -= 5;

	vector<ConvexCastReturnInfo> info;

	iPhysics::getInstance().convexCast(matrix, target, _collisionCast, iRayPreFilterDelegate(this, &CharacterController::onRayPreFilter), nullptr, info);

	if (info.size())
	{
		point = info[0]._point._vec;
		normal = info[0]._normal._vec;

		result = matrix._pos._y - point._y;
		result -= (_characterHeight * 0.5);
	}

	return result;
}

