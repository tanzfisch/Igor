#ifndef __BOSSDESTROYED__
#define __BOSSDESTROYED__

#include "Entity.h"
#include <iNode.h>

namespace Igor
{
    class iScene;
}

class BossDestroyed : public Entity
{

public:

    BossDestroyed(iScene* scene, const iaMatrixf& matrix);
    virtual ~BossDestroyed();

private:

    uint32 _fireBallNodeID = iNode::INVALID_NODE_ID;
    uint32 _smokeNodeID = iNode::INVALID_NODE_ID;
    uint32 _traceNodeID = iNode::INVALID_NODE_ID;
	uint32 _transformNodeID = iNode::INVALID_NODE_ID;

	iaVector3f _pos;

    void handle();
    iaVector3f updatePos();
    void hitBy(uint64 entityID);

};

#endif