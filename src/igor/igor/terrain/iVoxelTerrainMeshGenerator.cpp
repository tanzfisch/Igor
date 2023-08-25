#include <igor/terrain/iVoxelTerrainMeshGenerator.h>

#include <igor/terrain/data/iVoxelBlock.h>
#include <igor/generation/iContouringCubes.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/scene/nodes/iNodeMesh.h>
#include <igor/scene/nodes/iNodeLODSwitch.h>
#include <igor/resources/model/iModel.h>
#include <igor/resources/mesh/iMeshBuilder.h>
#include <igor/resources/material/iMaterialResourceFactory.h>

#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodePhysics.h>

// uncomment next line for voxel terrain debug using no physics
// #define DEBUG_VOXEL_TERRAIN_NO_PHYSICS

namespace igor
{

    iVoxelTerrainMeshGenerator::iVoxelTerrainMeshGenerator()
    {
        _identifier = "vtg";
        _name = "Voxel Terrain Mesh Generator";
    }

    iModelDataIO *iVoxelTerrainMeshGenerator::createInstance()
    {
        iVoxelTerrainMeshGenerator *result = new iVoxelTerrainMeshGenerator();
        return static_cast<iModelDataIO *>(result);
    }

    __IGOR_DISABLE_WARNING__(4100)
    iNodePtr iVoxelTerrainMeshGenerator::importData(const iaString &sectionName, iModelDataInputParameterPtr parameter)
    {
        const iVoxelTerrainTileInformation &tileInformation = std::any_cast<iVoxelTerrainTileInformation>(parameter->_parameters);

        iVoxelData *voxelData = tileInformation._voxelData;
        iVoxelData *voxelDataNextLOD = tileInformation._voxelDataNextLOD;

        const int64 width = voxelData->getWidth();
        const int64 height = voxelData->getHeight();
        const int64 depth = voxelData->getDepth();

        iNodePtr result = iNodeManager::getInstance().createNode<iNode>("group");

        iContouringCubes contouringCubes;
        contouringCubes.setVoxelData(voxelData);
        contouringCubes.setVoxelDataNextLOD(voxelDataNextLOD);
        contouringCubes.setNextLODVoxelOffset(tileInformation._voxelOffsetToNextLOD);

        iMeshPtr mesh = contouringCubes.compile(iaVector3I(), iaVector3I(width, height, depth), tileInformation._lod, tileInformation._neighboursLOD);        

        if (mesh.get() != nullptr)
        {
            mesh->setKeepRawData(true);
            
            iNodeMesh *meshNode = iNodeManager::getInstance().createNode<iNodeMesh>();
            meshNode->setMesh(mesh);
            meshNode->setMaterial(tileInformation._material);
            meshNode->setName(iaString("voxel_mesh_") + sectionName);
            meshNode->setVisible(false);

            meshNode->setTargetMaterial(tileInformation._targetMaterial);
            result->insertNode(meshNode);

#ifndef DEBUG_VOXEL_TERRAIN_NO_PHYSICS
            if (tileInformation._lod == 0)
            {
                iNodePhysics *physicsNode = iNodeManager::getInstance().createNode<iNodePhysics>();
                iaMatrixd offset;
                physicsNode->addMesh(mesh, 1, offset);
                physicsNode->finalizeCollision(true);
                physicsNode->setMaterial(tileInformation._physicsMaterialID);

                result->insertNode(physicsNode);
            }
#endif
        }

        delete voxelData;
        delete voxelDataNextLOD;

        return result;
    }
    __IGOR_ENABLE_WARNING__(4100)

} // namespace igor
