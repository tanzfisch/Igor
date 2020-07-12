#ifndef __DIGEFFECT__
#define __DIGEFFECT__

#include "GameObject.h"

#include <igor/scene/nodes/iNode.h>

namespace igor
{
    class iScene;
}

class DigEffect : public GameObject
{

public:
    DigEffect(iScenePtr scene, const iaMatrixd &matrix);
    virtual ~DigEffect();

private:
    uint32 _particleSystemNodeID = iNode::INVALID_NODE_ID;
    uint32 _transformNodeID = iNode::INVALID_NODE_ID;

    iaVector3d _pos;

    void handle();
    iaVector3d getCurrentPos();
    void hitBy(uint64 entityID);
};

#endif