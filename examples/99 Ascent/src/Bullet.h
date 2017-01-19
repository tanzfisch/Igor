#ifndef __BULLET__
#define __BULLET__

#include "GameObject.h"

namespace Igor
{
    class iScene;
    class iPhysicsBody;
}

enum class BulletType
{

};

class Bullet : public GameObject
{

    friend class Ascent;

public:

    void setForce(const iaVector3d& force);

private:

    Fraction _parentFraction = Fraction::None;
    uint32 _transformNodeID = 0;
	uint32 _particleSystemNodeID = 0;

    iaVector3d _force;

    void init();

    void deinit();

    void handle();
    iaVector3d updatePos();
    void hitBy(uint64 entityID);

    void onApplyForceAndTorque(iPhysicsBody* body, float32 timestep, int threadIndex);

    static iEntity* createInstance();

    Bullet();
    virtual ~Bullet() = default;


};

#endif