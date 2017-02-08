#ifndef __GRANADE__
#define __GRANADE__

#include "GameObject.h"

namespace Igor
{
    class iScene;
    class iPhysicsBody;
}

class Granade : public GameObject
{

public:

    Granade(iScene* scene, const iaMatrixd& matrix, Fraction fraction);
    virtual ~Granade();

private:

    Fraction _parentFraction = Fraction::None;
    uint32 _transformNodeID = 0;
    iScene* _scene = nullptr;

    iaVector3d _force;

    void calcDamage(float32& shield, float32& health);

    void handle();
    iaVector3d getCurrentPos();
    void hitBy(uint64 entityID);

    void onApplyForceAndTorque(iPhysicsBody* body, float32 timestep);

};

#endif