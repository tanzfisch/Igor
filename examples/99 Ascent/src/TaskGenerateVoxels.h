#ifndef __TASKGENERATEVOXELS__
#define __TASKGENERATEVOXELS__

#include <iTask.h>
using namespace Igor;

#include <iaVector3.h>
using namespace IgorAux;

namespace Igor
{
    class iVoxelData;
}

struct VoxelBlock
{
    bool _generatedVoxels = false;
    bool _generatedEnemies = false;
    iaVector3I _offset;
    iaVector3i _size;
    iVoxelData* _voxelData = nullptr;
};

class TaskGenerateVoxels : public iTask
{

public:

    /*! initializes member variables

    \param window window connected to render context
    */
    TaskGenerateVoxels(VoxelBlock* voxelBlock, uint32 priority);

    /*! does nothing
    */
    virtual ~TaskGenerateVoxels() = default;

protected:

    /*! runs the task
    */
    void run();

private:

    /*! the data to work with
    */
    VoxelBlock* _voxelBlock = nullptr;

};

#endif
