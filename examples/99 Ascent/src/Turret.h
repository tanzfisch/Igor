#ifndef __TURRET__
#define __TURRET__

#include "GameObject.h"

namespace Igor
{
    class iScene;
    class iNode;
    class iNodeTransform;
    class iPhysicsBody;
}

class Turret : public GameObject
{

    friend class Ascent;

public:

    static iaString TYPE_NAME;

    void setParentNode(iNodeTransform* parent);
    
    void setTargetID(uint64 targetID);

private:

    uint64 _targetID;
    uint32 _turretNodeID = 0;
    bool _initilized = false;

    uint32 _idleCounter = 0;
    uint32 _parentNodeID = 0;
    uint32 _platformID = 0;
    uint32 _headingID = 0;
    uint32 _pitchID = 0;

    float64 _time = 0;

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

    void updatePosition();

    static Entity* createInstance();

    Turret();
    virtual ~Turret();

};

#endif