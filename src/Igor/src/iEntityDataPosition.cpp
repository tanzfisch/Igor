#include <iEntityDataPosition.h>

namespace Igor
{

    iEntityDataPosition::iEntityDataPosition()
    {
        _octree = new iOctree(iAACubed(iaVector3d(0, 0, 0), 40000.0), 50.0, 8, 4);
    }

    iEntityDataPosition::~iEntityDataPosition()
    {
        _octree = new iOctree(iAACubed(iaVector3d(0, 0, 0), 40000.0), 50.0, 8, 4);
    }

}
