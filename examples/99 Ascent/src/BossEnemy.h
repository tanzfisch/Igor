#ifndef __BOSSENEMY__
#define __BOSSENEMY__

#include "Entity.h"

namespace Igor
{
    class iScene;
    class iPhysicsBody;
    class iPhysicsJoint;
}

class BossEnemy : public Entity
{
    
public:

	BossEnemy(iScene* scene, const iaMatrixd& matrix, uint64 playerID);
    virtual ~BossEnemy();

private:

    uint64 _playerID;
    uint64 _turretAID = 0;
	uint64 _turretBID = 0;
	uint64 _turretCID = 0;
	uint64 _turretDID = 0;
	uint64 _turretEID = 0;
	uint64 _turretFID = 0;

    uint32 _idleCounter = 0;
    uint32 _transformNodeID = 0;
	uint32 _physicsNodeID = 0;
    iScene* _scene = nullptr;

    void handle();
    iaVector3d updatePos();
    void hitBy(uint64 entityID);

};

#endif