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

public:

    Bullet(iScene* scene, const iaVector3d& addForce, const iaMatrixd& matrix, Fraction fraction);
    virtual ~Bullet();

private:

    Fraction _parentFraction = Fraction::None;
    uint32 _transformNodeID = 0;
	uint32 _particleSystemNodeID = 0;
    iScene* _scene = nullptr;

    iaVector3d _force;

    void handle();
    iaVector3d updatePos();
    void hitBy(uint64 entityID);

    void onApplyForceAndTorque(iPhysicsBody* body, float32 timestep);

};

#endif