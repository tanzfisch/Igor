#ifndef __BOSSENEMY__
#define __BOSSENEMY__

#include "GameObject.h"

namespace igor
{
    class iScene;
    class iPhysicsBody;
    class iPhysicsJoint;
    class iVoxelTerrain;
}

class BossEnemy : public GameObject
{
    
public:

	BossEnemy(iScene* scene, iVoxelTerrain* voxelTerrain, const iaMatrixd& matrix, uint64 playerID);
    virtual ~BossEnemy();

    iaVector3d getCurrentPos();

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
    iVoxelTerrain* _voxelTerrain = nullptr;

    void handle();
    void hitBy(uint64 entityID);

};

#endif