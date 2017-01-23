#ifndef __MUZZLEFLASH__
#define __MUZZLEFLASH__

#include "GameObject.h"
#include <iNode.h>

namespace Igor
{
    class iScene;
}

class MuzzleFlash : public GameObject
{

    friend class Ascent;

public:

    static iaString TYPE_NAME;

    /*! sets emitter node
    */
    void setEmitterNode(uint32 emitterNodeID);

private:

    /*! emitter node id
    */
    uint32 _emitterNodeID = iNode::INVALID_NODE_ID;

    /*! muzzle flash model node id
    */
    uint32 _muzzleFlashModelID = iNode::INVALID_NODE_ID;
    
    /*! muzzle smoke model node id
    */
    uint32 _muzzleSmokeModelID = iNode::INVALID_NODE_ID;

    /*! flash still running
    */
    bool _muzzleFlashRunning = true;

    /*! smoke still running
    */
    bool _muzzleSmokeRunning = true;

    /*! initialize entity
    */
    void init();

    /*! deinitialize entity
    */
    void deinit();

    /*!
    */
    void handle();

    void hitBy(uint64 entityID);

    void onMuzzleFlashLoaded();
    void onMuzzleSmokeLoaded();
    void onMuzzleFlashFinished();
    void onMuzzleSmokeFinished();

    static Entity* createInstance();

    MuzzleFlash();

    /*! does nothing
    */
    virtual ~MuzzleFlash() = default;

};

#endif