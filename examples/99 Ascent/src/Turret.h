#ifndef __TURRET__
#define __TURRET__

#include "GameObject.h"

namespace Igor
{
    class iScene;
    class iNodeTransform;
}

class Turret : public GameObject
{

public:

    Turret(iScene* scene, iNodeTransform* ancor, Fraction fraction, uint64 playerID);
    virtual ~Turret();

private:

    uint64 _playerID;
    uint32 _turretNodeID = 0;
    bool _initilized = false;

    uint32 _idleCounter = 0;
    uint32 _parentNodeID = 0;
    uint32 _platformID = 0;
    uint32 _headingID = 0;
    uint32 _pitchID = 0;
    iScene* _scene = nullptr;

    float64 _time = 0;

    void handle();
    iaVector3d getCurrentPos();
    void hitBy(uint64 entityID);

};

#endif