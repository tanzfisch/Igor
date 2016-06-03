#ifndef __GENERATORTERRAIN__
#define __GENERATORTERRAIN__

#include <iaVector3.h>
using namespace IgorAux;

#include <iModelDataIO.h>
using namespace Igor;

namespace Igor
{
    class iContouringCubes;
    class iVoxelData;
    class iMeshBuilder;
    class iTargetMaterial;
}

struct TileInformation
{
    iaVector3I _relativePos;
    iaVector3I _absolutePos;
    int64 _width = 0;
    int64 _depth = 0;
    int64 _height = 0;
    iVoxelData* _voxelData = nullptr;
    uint32 _lodTriggerID = 0;
    uint32 _materialID = 0;
};

class VoxelTerrainMeshGenerator : public iModelDataIO
{

public:

    /*! generates terrain tiles 

    !!! ATTENTION consumes and deletes "parameter"

    \param sectionname name of tile section
    \return parameter tile parameters
    */
    iNode* importData(const iaString& sectionName, iModelDataInputParameter* parameter);

    /*! initialize members
    */
    VoxelTerrainMeshGenerator();

    /*! does nothing
    */
    virtual ~VoxelTerrainMeshGenerator() = default;

    /*! creates an instance of this class

    \returns new instance
    */
    static iModelDataIO* createInstance();

};

#endif
