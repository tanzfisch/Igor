#ifndef __ENEMY__
#define __ENEMY__

#include "Entity.h"

namespace Igor
{
    class iScene;
    class iPhysicsBody;
}

class Enemy : public Entity
{

public:

    Enemy(iScene* scene, const iaVector3f& pos);
    virtual ~Enemy();

    void handle();

private:

    uint32 _idleCounter = 0;
    uint32 _transformNodeID = 0;
    uint64 _bodyID = 0;
    iScene* _scene = nullptr;

    void onApplyForceAndTorque(iPhysicsBody* body, float32 timestep, int threadIndex);


};

#endif