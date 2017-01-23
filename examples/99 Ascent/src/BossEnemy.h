#ifndef __BOSSENEMY__
#define __BOSSENEMY__

#include "GameObject.h"

namespace Igor
{
    class iScene;
    class iPhysicsBody;
    class iPhysicsJoint;
}

class BossEnemy : public GameObject
{
    
    friend class Ascent;

public:

    static iaString TYPE_NAME;

    void setTargetID(uint64 targetID);

    /*! sets position of entity

    \param position new position to set
    */
    void setPosition(const iaVector3d& position);

private:

    uint64 _targetID;
    uint64 _turretAID = 0;
	uint64 _turretBID = 0;
	uint64 _turretCID = 0;
	uint64 _turretDID = 0;
	uint64 _turretEID = 0;
	uint64 _turretFID = 0;

    uint32 _idleCounter = 0;
    uint32 _transformNodeID = 0;
	uint32 _physicsNodeID = 0;

    /*! called when hit by an other entity

    \param entityId the id of the entity that was colliding with this entity
    */
    void hitBy(uint64 entityID);

    /*! initialize entity
    */
    void init();

    /*! deinitialize entity
    */
    void deinit();

    /*! handle entity
    */
    void handle();

    static Entity* createInstance();

    BossEnemy();
    virtual ~BossEnemy();


};

#endif