#ifndef __GRANADE__
#define __GRANADE__

#include "Entity.h"

namespace Igor
{
    class iScene;
    class iPhysicsBody;
}

class Granade : public Entity
{

public:

    Granade(iScene* scene, const iaMatrixf& matrix, Fraction fraction);
    virtual ~Granade();

private:

    Fraction _parentFraction = Fraction::None;
    uint32 _transformNodeID = 0;
    iScene* _scene = nullptr;

    iaVector3f _force;

    void calcDamage(float32& shield, float32& health);

    void handle();
    iaVector3f updatePos();
    void hitBy(uint64 entityID);

    void onApplyForceAndTorque(iPhysicsBody* body, float32 timestep, int threadIndex);

};

#endif