#ifndef __DIGEFFECT__
#define __DIGEFFECT__

#include "GameObject.h"

#include <igor/graphics/scene/nodes/iNode.h>

namespace Igor
{
    class iScene;
}

class DigEffect : public GameObject
{

public:
    DigEffect(iScene *scene, const iaMatrixd &matrix);
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