#include "MuzzleFlash.h"

#include <iNodeManager.h>
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

MuzzleFlash::MuzzleFlash(iScene* scene, uint32 emitterID)
    : GameObject(Fraction::None, GameObjectType::None)
{
    _emitterNodeID = emitterID;

    setHealth(100.0);
    setDamage(0.0);
    setShieldDamage(0.0);
    
    iNodeModel* particleSystem1 = iNodeManager::getInstance().createNode<iNodeModel>();
    _muzzleFlashModelID = particleSystem1->getID();
    particleSystem1->setModel("MuzzleFlash.ompf");
    particleSystem1->registerModelReadyDelegate(iModelReadyDelegate(this, &MuzzleFlash::onMuzzleFlashLoaded));
    scene->getRoot()->insertNode(particleSystem1);

    iNodeModel* particleSystem2 = iNodeManager::getInstance().createNode<iNodeModel>();
    _muzzleSmokeModelID = particleSystem2->getID();
    particleSystem2->setModel("MuzzleSmoke.ompf");
    particleSystem2->registerModelReadyDelegate(iModelReadyDelegate(this, &MuzzleFlash::onMuzzleSmokeLoaded));
    scene->getRoot()->insertNode(particleSystem2);
    
    iNodePtr emitterNode = iNodeManager::getInstance().getNode(_emitterNodeID);
    if (emitterNode != nullptr)
    {
        iaMatrixd matrix;
        emitterNode->calcWorldTransformation(matrix);
        _pos = matrix._pos;
    }
}

MuzzleFlash::~MuzzleFlash()
{
    iNodeManager::getInstance().destroyNodeAsync(_muzzleFlashModelID);
    iNodeManager::getInstance().destroyNodeAsync(_muzzleSmokeModelID);
}

void MuzzleFlash::onMuzzleFlashLoaded(uint64 nodeID)
{
    iNodePtr emitterNode = iNodeManager::getInstance().getNode(_emitterNodeID);
    if (emitterNode != nullptr)
    {
        iNodeModel* muzzleFlashNode = static_cast<iNodeModel*>(iNodeManager::getInstance().getNode(_muzzleFlashModelID));
        iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(muzzleFlashNode->getChild("ParticleSystem"));
        if (particleSystem != nullptr)
        {
            particleSystem->setEmitter(_emitterNodeID);
            particleSystem->start();
            particleSystem->registerParticleSystemFinishedDelegate(iParticleSystemFinishedDelegate(this, &MuzzleFlash::onMuzzleFlashFinished));
        }
    }
}

void MuzzleFlash::onMuzzleFlashFinished()
{
    _muzzleFlashRunning = false;
}

void MuzzleFlash::onMuzzleSmokeFinished()
{
    _muzzleSmokeRunning = false;
}

void MuzzleFlash::onMuzzleSmokeLoaded(uint64 nodeID)
{
    iNodePtr emitterNode = iNodeManager::getInstance().getNode(_emitterNodeID);
    if (emitterNode != nullptr)
    {
        iNodeModel* muzzleSmokeNode = static_cast<iNodeModel*>(iNodeManager::getInstance().getNode(_muzzleSmokeModelID));
        iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(muzzleSmokeNode->getChild("ParticleSystem"));
        if (particleSystem != nullptr)
        {
            particleSystem->setEmitter(_emitterNodeID);
            particleSystem->start();
            particleSystem->registerParticleSystemFinishedDelegate(iParticleSystemFinishedDelegate(this, &MuzzleFlash::onMuzzleSmokeFinished));
        }
    }
}

void MuzzleFlash::handle()
{
    if (!_muzzleFlashRunning &&
        !_muzzleSmokeRunning)
    {
        kill();
    }
}

void MuzzleFlash::hitBy(uint64 entityID)
{
    // nothing to do
}

iaVector3d MuzzleFlash::getCurrentPos()
{
    return _pos;
}

