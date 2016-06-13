#ifndef __BULLETHIT__
#define __BULLETHIT__

#include "Entity.h"
#include <iNode.h>

namespace Igor
{
    class iScene;
}

class BulletHit : public Entity
{

public:

	BulletHit(iScene* scene, const iaMatrixf& matrix);
    virtual ~BulletHit();

private:

    uint32 _particleSystemNodeID = iNode::INVALID_NODE_ID;
	uint32 _emitterNodeID = iNode::INVALID_NODE_ID;

	iaVector3f _pos;

    void handle();
    iaVector3f updatePos();
    void hitBy(uint64 entityID);

};

#endif