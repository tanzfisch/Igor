#ifndef __PLAYER__
#define __PLAYER__

#include "Entity.h"

namespace Igor
{
    class iScene;
    class iPhysicsBody;
}

class Player : public Entity
{

public:

    Player(iScene* scene, const iaVector3f& pos);
    virtual ~Player();

    void startUp();
    void stopUp();
    void startDown();
    void stopDown();
    void startLeft();
    void stopLeft();
    void startRight();
    void stopRight();

    void handle();

private:

    bool _up = false;
    bool _down = false;
    bool _left = false;
    bool _right = false;

    uint32 _transformNodeID = 0;
    uint64 _bodyID = 0;

    iScene* _scene = nullptr;

    void onApplyForceAndTorque(iPhysicsBody* body, float32 timestep, int threadIndex);


};

#endif