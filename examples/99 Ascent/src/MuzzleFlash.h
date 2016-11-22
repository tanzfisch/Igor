#ifndef __MUZZLEFLASH__
#define __MUZZLEFLASH__

#include "Entity.h"
#include <iNode.h>

namespace Igor
{
    class iScene;
}

class MuzzleFlash : public Entity
{

public:

    MuzzleFlash(iScene* scene, uint32 emitterID);
    virtual ~MuzzleFlash();

private:

    uint32 _emitterNodeID = iNode::INVALID_NODE_ID;
    uint32 _muzzleFlashModelID = iNode::INVALID_NODE_ID;
    uint32 _muzzleSmokeModelID = iNode::INVALID_NODE_ID;

	iaVector3f _pos;

    void onMuzzleFlashLoaded();
    void onMuzzleSmokeLoaded();
    void onMuzzleFlashFinished();
    void onMuzzleSmokeFinished();

    bool _muzzleFlashRunning = true;
    bool _muzzleSmokeRunning = true;

    void handle();
    iaVector3f updatePos();
    void hitBy(uint64 entityID);

};

#endif