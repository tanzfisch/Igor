#ifndef __MUZZLEFLASH__
#define __MUZZLEFLASH__

#include "GameObject.h"
#include <igor/graphics/scene/nodes/iNode.h>

namespace Igor
{
    class iScene;
}

class MuzzleFlash : public GameObject
{

public:
    MuzzleFlash(iScene *scene, uint32 emitterID);
    virtual ~MuzzleFlash();

private:
    uint32 _emitterNodeID = iNode::INVALID_NODE_ID;
    uint32 _muzzleFlashModelID = iNode::INVALID_NODE_ID;
    uint32 _muzzleSmokeModelID = iNode::INVALID_NODE_ID;

    iaVector3d _pos;

    void onMuzzleFlashLoaded(uint64 nodeID);
    void onMuzzleSmokeLoaded(uint64 nodeID);
    void onMuzzleFlashFinished();
    void onMuzzleSmokeFinished();

    bool _muzzleFlashRunning = true;
    bool _muzzleSmokeRunning = true;

    void handle();
    iaVector3d getCurrentPos();
    void hitBy(uint64 entityID);
};

#endif