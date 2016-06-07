#ifndef __ENEMY__
#define __ENEMY__

#include "Entity.h"

namespace Igor
{
    class iScene;
    class iPhysicsBody;
    class iPhysicsJoint;
}

class Enemy : public Entity
{
    
public:

    Enemy(iScene* scene, const iaMatrixf& matrix);
    virtual ~Enemy();

private:

    uint64 _turretAID = 0;
    uint64 _turretBID = 0;

    uint32 _idleCounter = 0;
    uint32 _transformNodeID = 0;
	uint32 _physicsNodeID = 0;
    iScene* _scene = nullptr;

    iaVector3f _force;

    void handle();
    iaVector3f updatePos();
    void hitBy(uint64 entityID);

    void onApplyForceAndTorque(iPhysicsBody* body, float32 timestep, int threadIndex);
};

#endif