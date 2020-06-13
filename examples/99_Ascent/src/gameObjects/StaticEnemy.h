#ifndef __STATICENEMY__
#define __STATICENEMY__

#include "GameObject.h"

namespace igor
{
    class iScene;
    class iPhysicsBody;
    class iPhysicsJoint;
    class iVoxelTerrain;
}

class StaticEnemy : public GameObject
{
    
public:

    StaticEnemy(iScene* scene, iVoxelTerrain* voxelTerrain, const iaMatrixd& matrix, uint64 playerID);
    virtual ~StaticEnemy();

private:

    uint64 _playerID;
    uint64 _turretID = 0;

    uint32 _idleCounter = 0;
    uint32 _transformNodeID = 0;
	uint32 _physicsNodeID = 0;
    iScene* _scene = nullptr;
    iVoxelTerrain* _voxelTerrain = nullptr;

    void handle();
    iaVector3d getCurrentPos();
    void hitBy(uint64 entityID);

};

#endif