#include "DigEffect.h"

#include <iNodeFactory.h>
#include <iNodeTransform.h>
#include <iNodePhysics.h>
#include <iNodeModel.h>
#include <iModel.h>
#include <iScene.h>
#include <iPhysics.h>
#include <iPhysicsBody.h>
#include <iPhysicsCollision.h>
#include <iNodeParticleSystem.h>
#include <iNodeEmitter.h>
#include <iMaterialResourceFactory.h>
#include <iMaterial.h>
using namespace Igor;

#include <iaString.h>
using namespace IgorAux;

#include "EntityManager.h"

DigEffect::DigEffect(iScene* scene, const iaMatrixf& matrix)
    : Entity(Fraction::None, EntityType::None)
{
    setHealth(100.0);
    setDamage(0.0);
    setShieldDamage(0.0);

    iGradientui emission;
    emission.insertValue(0.0, 5);
    emission.insertValue(0.1, 0);

    iGradientColor4f colorGradient;
    colorGradient.insertValue(0.0, iaColor4f(0.6, 0.6, 0.6, 0));
    colorGradient.insertValue(0.0, iaColor4f(0.6, 0.6, 0.6, 0.5));
    colorGradient.insertValue(0.5, iaColor4f(0.6, 0.6, 0.6, 0.5));
    colorGradient.insertValue(1.0, iaColor4f(0.6, 0.6, 0.6, 0));

    iGradientVector2f velocity;
    velocity.insertValue(0.0, iaVector2f(0.01, 0.02));

    iGradientVector2f visibility;
    visibility.insertValue(0.0, iaVector2f(0.5, 1.5));

    iGradientVector2f size;
    size.insertValue(0.0, iaVector2f(3.0, 4.0));

    iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeParticleSystem));
    _traceNodeID = particleSystem->getID();
    particleSystem->setMaterial(iMaterialResourceFactory::getInstance().getMaterialID("PMat"));
    particleSystem->setTextureA("particleSmoke.png");
    particleSystem->setTextureB("octave1.png");
    particleSystem->setTextureC("octave2.png");
    particleSystem->setLoop(false);
    particleSystem->setColorGradient(colorGradient);
    particleSystem->setStartVelocityGradient(velocity);
    particleSystem->setStartVisibleTimeGradient(visibility);
    particleSystem->setStartSizeGradient(size);
    particleSystem->setEmissionGradient(emission);
    particleSystem->setPeriodTime(4.0);
    particleSystem->start();

	iNodeEmitter* emitter = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeEmitter));
	emitter->setType(iEmitterType::Sphere);
    emitter->setSize(5);
	particleSystem->setEmitter(emitter->getID());

	iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	transformNode->setMatrix(matrix);
	_pos = matrix._pos;

	scene->getRoot()->insertNode(particleSystem);
    
    transformNode->insertNode(emitter);
	scene->getRoot()->insertNode(transformNode);
}

DigEffect::~DigEffect()
{
    if (_traceNodeID != iNode::INVALID_NODE_ID)
    {
        iNodeFactory::getInstance().destroyNode(_traceNodeID);
    }

	if (_transformNodeID != iNode::INVALID_NODE_ID)
	{
		iNodeFactory::getInstance().destroyNode(_transformNodeID);
	}
}

void DigEffect::hitBy(uint64 entityID)
{
	// nothing to do
}

iaVector3f DigEffect::updatePos()
{
	return _pos;
}

void DigEffect::handle()
{
    iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().getNode(_traceNodeID));

	if (particleSystem != nullptr)
	{
		if (particleSystem->isFinished())
		{
			kill();
		}
	}
}