#ifndef __TURRET__
#define __TURRET__

#include "GameObject.h"

#include <igor/graphics/scene/nodes/iNode.h>
using namespace Igor;

#include <iaux/system/iaTime.h>
using namespace IgorAux;

namespace Igor
{
    class iScene;
    class iNodeTransform;
    class iVoxelTerrain;
} // namespace Igor

class Turret : public GameObject
{

public:
    Turret(iScene *scene, iNodeTransform *ancor, iVoxelTerrain *voxelTerrain, Fraction fraction, uint64 playerID);
    virtual ~Turret();

private:
    uint64 _playerID;
    uint32 _turretNodeID = iNode::INVALID_NODE_ID;
    bool _initilized = false;

    uint32 _idleCounter = 0;
    uint32 _parentNodeID = 0;
    uint32 _platformID = 0;
    uint32 _headingID = 0;
    uint32 _pitchID = 0;
    iScene *_scene = nullptr;
    iVoxelTerrain *_voxelTerrain = nullptr;

    iaTime _time;

    void handle();
    iaVector3d getCurrentPos();
    void hitBy(uint64 entityID);
    void onModelReady(uint64 modelNodeID);
};

#endif