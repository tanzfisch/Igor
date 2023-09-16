#include <igor/terrain/iVoxelTerrainMeshGenerator.h>

#include <igor/terrain/data/iVoxelBlock.h>
#include <igor/generation/iContouringCubes.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/scene/nodes/iNodeMesh.h>
#include <igor/scene/nodes/iNodeLODSwitch.h>
#include <igor/resources/mesh/iMeshBuilder.h>

#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodePhysics.h>

// uncomment next line for voxel terrain debug using no physics
// #define DEBUG_VOXEL_TERRAIN_NO_PHYSICS

namespace igor
{

    iVoxelTerrainMeshGenerator::iVoxelTerrainMeshGenerator()
    {
        _identifier = "igor.vtg";
        _name = "Voxel Terrain Mesh Generator";
    }

    iModelDataIO *iVoxelTerrainMeshGenerator::createInstance()
    {
        iVoxelTerrainMeshGenerator *result = new iVoxelTerrainMeshGenerator();
        return static_cast<iModelDataIO *>(result);
    }

    __IGOR_DISABLE_WARNING__(4100)
    iNodePtr iVoxelTerrainMeshGenerator::importData(const iParameters &parameters)
    {
        iVoxelData* voxelData = parameters.getParameter<iVoxelData*>("voxelData", nullptr);
        iVoxelData* voxelDataNextLOD = parameters.getParameter<iVoxelData*>("voxelDataNextLOD", nullptr);
        
        iMaterialPtr material = parameters.getParameter<iMaterialPtr>("material", nullptr);
        iTargetMaterialPtr targetMaterial = parameters.getParameter<iTargetMaterialPtr>("targetMaterial", nullptr);
        const uint32 lod = parameters.getParameter<uint32>("lod", 0);
        const uint8 neighboursLOD = parameters.getParameter<uint8>("neighboursLOD", 0);
        const iaVector3I voxelOffsetToNextLOD = parameters.getParameter<iaVector3I>("voxelOffsetToNextLOD", iaVector3I());
        const iaString sectionName = parameters.getParameter<iaString>("name", "");
        const uint64 physicsMaterialID = parameters.getParameter<uint64>("physicsMaterialID", 0);

        const int64 width = voxelData->getWidth();
        const int64 height = voxelData->getHeight();
        const int64 depth = voxelData->getDepth();

        iNodePtr result = iNodeManager::getInstance().createNode<iNode>("group");

        iContouringCubes contouringCubes;
        contouringCubes.setVoxelData(voxelData);
        contouringCubes.setVoxelDataNextLOD(voxelDataNextLOD);
        contouringCubes.setNextLODVoxelOffset(voxelOffsetToNextLOD);

        iMeshPtr mesh = contouringCubes.compile(iaVector3I(), iaVector3I(width, height, depth), lod, neighboursLOD);        

        if (mesh.get() != nullptr)
        {
            mesh->setKeepRawData(true);
            
            iNodeMesh *meshNode = iNodeManager::getInstance().createNode<iNodeMesh>();
            meshNode->setMesh(mesh);
            meshNode->setMaterial(material);
            meshNode->setName(iaString("voxel_mesh_") + sectionName);
            meshNode->setVisible(false);

            meshNode->setTargetMaterial(targetMaterial);
            result->insertNode(meshNode);

#ifndef DEBUG_VOXEL_TERRAIN_NO_PHYSICS
            if (lod == 0)
            {
                iNodePhysics *physicsNode = iNodeManager::getInstance().createNode<iNodePhysics>();
                iaMatrixd offset;
                physicsNode->addMesh(mesh, 1, offset);
                physicsNode->finalizeCollision(true);
                physicsNode->setMaterial(physicsMaterialID);

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
