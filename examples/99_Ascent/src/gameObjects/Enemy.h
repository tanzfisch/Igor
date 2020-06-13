#ifndef __ENEMY__
#define __ENEMY__

#include "GameObject.h"

namespace igor
{
    class iScene;
    class iPhysicsBody;
    class iPhysicsJoint;
    class iVoxelTerrain;
}

class Enemy : public GameObject
{
    
public:

    Enemy(iScene* scene, iVoxelTerrain* voxelTerrain, const iaMatrixd& matrix, uint64 playerID);
    virtual ~Enemy();

private:

    uint64 _turretAID = 0;
    uint64 _turretBID = 0;

    uint32 _idleCounter = 0;
    uint32 _transformNodeID = 0;
    iScene* _scene = nullptr;
    iVoxelTerrain* _voxelTerrain = nullptr;

    iaVector3d _force;

    uint64 _playerID;

    void handle();
    iaVector3d getCurrentPos();
    void hitBy(uint64 entityID);

    void onApplyForceAndTorque(iPhysicsBody* body, float32 timestep);
};

#endif