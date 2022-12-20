#ifndef __PLAYER__
#define __PLAYER__

#include "GameObject.h"

#include <igor/scene/nodes/iNode.h>
using namespace igor;

#include <iaux/math/iaMatrix.h>
#include <iaux/system/iaTime.h>
using namespace iaux;

namespace igor
{
    class iScene;
    class iView;
    class iPhysicsBody;
    class iPhysicsJoint;
    class iView;
    class iWindow;
} // namespace igor

class Player : public GameObject
{

public:
    Player(iScenePtr scene, iView *view, const iaMatrixd &matrix);
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

    void shootPrimaryWeapon(iView &view, const iaVector3d &screenCoordinates);
    void shootSecondaryWeapon(iView &view, const iaVector3d &screenCoordinates);
    iaVector3I getGunPointPosition();

    void rotate(float32 heading, float32 pitch);

    void drawReticle(iWindowPtr window);

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

    iaTime _primaryWeaponTime;

    uint32 _materialSolid = 0;

    iScenePtr _scene = nullptr;

    iaVector3d _force;
    iaVector3d _torque;

    void handle();
    iaVector3d getCurrentPos();
    void hitBy(uint64 entityID);

    void onApplyForceAndTorque(iPhysicsBody *body, float32 timestep);
};

#endif