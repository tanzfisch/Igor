#ifndef __ENEMYDESTROYED__
#define __ENEMYDESTROYED__

#include "GameObject.h"

#include <igor/graphics/scene/nodes/iNode.h>

namespace Igor
{
    class iScene;
}

class EnemyDestroyed : public GameObject
{

public:
    EnemyDestroyed(iScene *scene, const iaMatrixd &matrix);
    virtual ~EnemyDestroyed();

private:
    uint32 _fireBallNodeID = iNode::INVALID_NODE_ID;
    uint32 _smokeNodeID = iNode::INVALID_NODE_ID;
    uint32 _traceNodeID = iNode::INVALID_NODE_ID;
    uint32 _transformNodeID = iNode::INVALID_NODE_ID;

    iaVector3d _pos;

    void handle();
    iaVector3d getCurrentPos();
    void hitBy(uint64 entityID);
};

#endif