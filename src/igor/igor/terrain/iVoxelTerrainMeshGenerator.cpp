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

// uncomment next line for voxel terrain debug coloring
// #define DEBUG_VOXEL_TERRAIN_COLORING

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
    iNodePtr iVoxelTerrainMeshGenerator::importData(const iaString &sectionName, iModelDataInputParameter *parameter)
    {
        const iVoxelTerrainTileInformation &tileInformation = std::any_cast<iVoxelTerrainTileInformation>(parameter->_parameters);
    
        iVoxelData *voxelData = tileInformation._voxelData;
        iVoxelData *voxelDataNextLOD = tileInformation._voxelDataNextLOD;

        int64 width = voxelData->getWidth();
        int64 height = voxelData->getHeight();
        int64 depth = voxelData->getDepth();

        iNodePtr result = iNodeManager::getInstance().createNode<iNode>();
        result->setName("group");

        iContouringCubes contouringCubes;
        contouringCubes.setVoxelData(voxelData);
        contouringCubes.setVoxelDataNextLOD(voxelDataNextLOD);
        contouringCubes.setNextLODVoxelOffset(tileInformation._voxelOffsetToNextLOD);

        iMeshPtr mesh = contouringCubes.compile(iaVector3I(), iaVector3I(width, height, depth), tileInformation._lod, tileInformation._neighboursLOD);

        if (mesh.get() != nullptr)
        {
            iNodeMesh *meshNode = iNodeManager::getInstance().createNode<iNodeMesh>();
            meshNode->setMesh(mesh);
            // TODO meshNode->setMaterial(tileInformation._materialID);
            meshNode->setName("mesh");
            meshNode->setVisible(false);

#ifdef DEBUG_VOXEL_TERRAIN_COLORING
            iaRandomNumberGenerator rand;
            rand.setSeed(reinterpret_cast<uint32>(voxelData));
            float32 r = ((rand.getNext() % 30) + 35.0f) / 100.0f;
            float32 g = ((rand.getNext() % 30) + 35.0f) / 100.0f;
            float32 b = ((rand.getNext() % 30) + 35.0f) / 100.0f;
            tileInformation._targetMaterial->setAmbient(iaColor3f(r * 0.8f, g * 0.8f, b * 0.8f));

            switch (tileInformation._lod)
            {
            case 0:
                tileInformation._targetMaterial->setDiffuse(iaColor3f(0.7f, 0.7f, 0.7f));
                break;
            case 1:
                tileInformation._targetMaterial->setDiffuse(iaColor3f(0.7f, 0.0f, 0.0f));
                break;
            case 2:
                tileInformation._targetMaterial->setDiffuse(iaColor3f(0.0f, 0.7f, 0.0f));
                break;
            case 3:
                tileInformation._targetMaterial->setDiffuse(iaColor3f(0.0f, 0.0f, 0.7f));
                break;
            case 4:
                tileInformation._targetMaterial->setDiffuse(iaColor3f(0.7f, 0.0f, 0.7f));
                break;
            case 5:
                tileInformation._targetMaterial->setDiffuse(iaColor3f(0.7f, 0.7f, 0.0f));
                break;
            case 6:
                tileInformation._targetMaterial->setDiffuse(iaColor3f(0.0f, 0.7f, 0.7f));
                break;
            case 7:
                tileInformation._targetMaterial->setDiffuse(iaColor3f(0.7f, 0.0f, 0.0f));
                break;
            case 8:
                tileInformation._targetMaterial->setDiffuse(iaColor3f(0.0f, 0.7f, 0.f));
                break;
            case 9:
                tileInformation._targetMaterial->setDiffuse(iaColor3f(0.0f, 0.0f, 0.7f));
                break;
            }
#endif

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
