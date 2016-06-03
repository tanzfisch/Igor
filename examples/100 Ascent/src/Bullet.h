#ifndef __BULLET__
#define __BULLET__

#include "Entity.h"

namespace Igor
{
    class iScene;
    class iPhysicsBody;
}

enum class BulletType
{

};

class Bullet : public Entity
{

public:

    Bullet(iScene* scene, const iaVector3f& addForce, const iaMatrixf& matrix, Fraction fraction);
    virtual ~Bullet();

private:

    Fraction _parentFraction = Fraction::None;
    uint32 _transformNodeID = 0;
	uint32 _particleSystemNodeID = 0;
    iScene* _scene = nullptr;

    iaVector3f _force;

    void handle();
    iaVector3f updatePos();
    void hitBy(uint64 entityID);

    void onApplyForceAndTorque(iPhysicsBody* body, float32 timestep, int threadIndex);

};

#endif