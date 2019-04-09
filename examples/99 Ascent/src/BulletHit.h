#ifndef __BULLETHIT__
#define __BULLETHIT__

#include "GameObject.h"

#include <iNode.h>
using namespace Igor;

namespace Igor
{
    class iScene;
}

class BulletHit : public GameObject
{

public:

	BulletHit(iScene* scene, const iaMatrixd& matrix);
    virtual ~BulletHit();

private:

    uint32 _particleSystemNodeID = iNode::INVALID_NODE_ID;
	uint32 _emitterNodeID = iNode::INVALID_NODE_ID;

	iaVector3d _pos;

    void handle();
    iaVector3d getCurrentPos();
    void hitBy(uint64 entityID);

};

#endif