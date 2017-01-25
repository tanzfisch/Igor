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

    /*! emitter node id
    */
    uint32 _emitterNodeID = iNode::INVALID_NODE_ID;

    Fraction _parentFraction = Fraction::None;
    uint32 _transformNodeID = 0;
	uint32 _particleSystemNodeID = 0;

    iaVector3d _force;

    iaMatrixd _matrix;

    void init();

    void deinit();

    void handle();

    void hitBy(uint64 entityID);

    void onApplyForceAndTorque(iPhysicsBody* body, float32 timestep);

    void onEffectLoaded();

    static Entity* createInstance();

    Bullet();
    virtual ~Bullet() = default;


};

#endif