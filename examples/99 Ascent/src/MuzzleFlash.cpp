#include "MuzzleFlash.h"

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

MuzzleFlash::MuzzleFlash(iScene* scene, uint32 emitterID)
    : Entity(Fraction::None, EntityType::None)
{
    _emitterNodeID = emitterID;

    setHealth(100.0);
    setDamage(0.0);
    setShieldDamage(0.0);
    
    iNodeModel* particleSystem1 = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    _muzzleFlashModelID = particleSystem1->getID();
    particleSystem1->setModel("MuzzleFlash.ompf");
    particleSystem1->registerModelLoadedDelegate(iModelLoadedDelegate(this, &MuzzleFlash::onMuzzleFlashLoaded));
    scene->getRoot()->insertNode(particleSystem1);

    iNodeModel* particleSystem2 = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    _muzzleSmokeModelID = particleSystem2->getID();
    particleSystem2->setModel("MuzzleSmoke.ompf");
    particleSystem2->registerModelLoadedDelegate(iModelLoadedDelegate(this, &MuzzleFlash::onMuzzleSmokeLoaded));
    scene->getRoot()->insertNode(particleSystem2);

    iNode* emitterNode = iNodeFactory::getInstance().getNode(_emitterNodeID);
    if (emitterNode != nullptr)
    {
        iaMatrixf matrix;
        emitterNode->calcWorldTransformation(matrix);
        _pos = matrix._pos;
    }
}

MuzzleFlash::~MuzzleFlash()
{
    iNodeFactory::getInstance().destroyNodeAsync(_muzzleFlashModelID);
    iNodeFactory::getInstance().destroyNodeAsync(_muzzleSmokeModelID);
}

void MuzzleFlash::onMuzzleFlashLoaded()
{
    iNode* emitterNode = iNodeFactory::getInstance().getNode(_emitterNodeID);
    if (emitterNode != nullptr)
    {
        iNodeModel* muzzleFlashNode = static_cast<iNodeModel*>(iNodeFactory::getInstance().getNode(_muzzleFlashModelID));
        iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(muzzleFlashNode->getChild("MuzzleFlash"));
        particleSystem->setEmitter(_emitterNodeID);
        particleSystem->start();
        particleSystem->registerParticleSystemFinishedDelegate(iParticleSystemFinishedDelegate(this, &MuzzleFlash::onMuzzleFlashFinished));
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

void MuzzleFlash::onMuzzleSmokeLoaded()
{
    iNode* emitterNode = iNodeFactory::getInstance().getNode(_emitterNodeID);
    if (emitterNode != nullptr)
    {
        iNodeModel* muzzleSmokeNode = static_cast<iNodeModel*>(iNodeFactory::getInstance().getNode(_muzzleSmokeModelID));
        iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(muzzleSmokeNode->getChild("MuzzleSmoke"));
        particleSystem->setEmitter(_emitterNodeID);
        particleSystem->start();
        particleSystem->registerParticleSystemFinishedDelegate(iParticleSystemFinishedDelegate(this, &MuzzleFlash::onMuzzleSmokeFinished));
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

iaVector3f MuzzleFlash::updatePos()
{
    return _pos;
}

