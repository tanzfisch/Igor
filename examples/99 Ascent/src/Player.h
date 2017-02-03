#ifndef __PLAYER__
#define __PLAYER__

#include "Entity.h"

#include <iNode.h>
#include <iaMatrix.h>
using namespace IgorAux;

namespace Igor
{
    class iScene;
    class iPhysicsBody;
    class iPhysicsJoint;
    class iView;
    class iWindow;
}

class Player : public Entity
{

public:

    Player(iScene* scene, const iaMatrixd& matrix);
    virtual ~Player();

    void startUp();
    void stopUp();
    void startDown();
    void stopDown();

    void startForward();
    void stopForward();
    void startBackward();
    void stopBackward();

    void startLeft();
    void stopLeft();
    void startRight();
    void stopRight();

    void startRollLeft();
    void stopRollLeft();
    void startRollRight();
    void stopRollRight();

    void startFastTurn();
    void stopFastTurn();

    void shootPrimaryWeapon(iView& view, const iaVector3d& screenCoordinates);
    void shootSecondaryWeapon(iView& view, const iaVector3d& screenCoordinates);
    void dig(uint64 toolSize, uint8 toolDensity);
    iaVector3I getGunPointPosition();

    void rotate(float32 heading, float32 pitch);

    void drawReticle(const iWindow& window);

    uint32 getLODTriggerID();

private:

    bool _up = false;
    bool _forward = false;
    bool _backward = false;
    bool _down = false;
    bool _left = false;
    bool _right = false;
    bool _rollLeft = false;
    bool _rollRight = false;
    bool _fastTurn = false;

	uint32 _physicsNodeID = iNode::INVALID_NODE_ID;
    uint32 _lodTriggerID = iNode::INVALID_NODE_ID;
    uint32 _transformNodeID = iNode::INVALID_NODE_ID;
    uint32 _transformRecoilLeftGun = iNode::INVALID_NODE_ID;
    uint32 _transformRecoilRightGun = iNode::INVALID_NODE_ID;
	uint32 _transformCamNodeID = iNode::INVALID_NODE_ID;
    uint32 _cameraNodeID = iNode::INVALID_NODE_ID;
    uint32 _emitterRightGunNodeID = iNode::INVALID_NODE_ID;
    uint32 _emitterLeftGunNodeID = iNode::INVALID_NODE_ID;

    float64 _primaryWeaponTime = 0;

    uint32 _materialSolid = 0;

    iScene* _scene = nullptr;

    iaVector3d _force;
    iaVector3d _torque;

    void handle();
    iaVector3d updatePos();
    void hitBy(uint64 entityID);

    void onApplyForceAndTorque(iPhysicsBody* body, float32 timestep);

};

#endif