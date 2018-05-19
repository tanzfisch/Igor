#include <iVoxelTerrainMeshGenerator.h>

#include <iVoxelData.h>
#include <iContouringCubes.h>
#include <iNodeFactory.h>
#include <iNodeMesh.h>
#include <iNodeLODSwitch.h>
#include <iModel.h>
#include <iaMemBlock.h>
#include <iMeshBuilder.h>
#include <iMaterialResourceFactory.h>
#include <iTextureResourceFactory.h>
#include <iTargetMaterial.h>
#include <iNodeTransform.h>
#include <iNodePhysics.h>

// uncomment next line for voxel terrain debug coloring
// #define DEBUG_VOXEL_TERRAIN_COLORING

// uncomment next line for voxel terrain debug using no physics
// #define DEBUG_VOXEL_TERRAIN_NO_PHYSICS

namespace Igor
{

    iVoxelTerrainMeshGenerator::iVoxelTerrainMeshGenerator()
    {
        _identifier = "vtg";
        _name = "Voxel Terrain Mesh Generator";
    }

    iModelDataIO* iVoxelTerrainMeshGenerator::createInstance()
    {
        iVoxelTerrainMeshGenerator* result = new iVoxelTerrainMeshGenerator();
        return static_cast<iModelDataIO*>(result);
    }

    __IGOR_DISABLE_WARNING__(4100)
        iNode* iVoxelTerrainMeshGenerator::importData(const iaString& sectionName, iModelDataInputParameter* parameter)
    {
        iVoxelTerrainTileInformation* tileInformation = reinterpret_cast<iVoxelTerrainTileInformation*>(parameter->_parameters.getDataPointer());

        iVoxelData* voxelData = tileInformation->_voxelData;
        iVoxelData* voxelDataNextLOD = tileInformation->_voxelDataNextLOD;

        int64 width = voxelData->getWidth();
        int64 height = voxelData->getHeight();
        int64 depth = voxelData->getDepth();

        iNode* result = iNodeFactory::getInstance().createNode(iNodeType::iNode);
        result->setName("group");

        iContouringCubes contouringCubes;
        contouringCubes.setVoxelData(voxelData);
        contouringCubes.setVoxelDataNextLOD(voxelDataNextLOD);
        contouringCubes.setNextLODVoxelOffset(tileInformation->_voxelOffsetToNextLOD);

        shared_ptr<iMesh> mesh = contouringCubes.compile(iaVector3I(), iaVector3I(width, height, depth), tileInformation->_lod, tileInformation->_neighboursLOD);

        if (mesh.get() != nullptr)
        {
            iNodeMesh* meshNode = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
            meshNode->setMesh(mesh);
            meshNode->setMaterial(tileInformation->_materialID);
            meshNode->setName("mesh");
            meshNode->setVisible(false);

#ifdef DEBUG_VOXEL_TERRAIN_COLORING
            iaRandomNumberGenerator rand;
            rand.setSeed(reinterpret_cast<uint32>(voxelData));
            float32 r = ((rand.getNext() % 30) + 35.0f) / 100.0f;
            float32 g = ((rand.getNext() % 30) + 35.0f) / 100.0f;
            float32 b = ((rand.getNext() % 30) + 35.0f) / 100.0f;
            tileInformation->_targetMaterial->setAmbient(iaColor3f(r * 0.8f, g* 0.8f, b* 0.8f));

            switch (tileInformation->_lod)
            {
            case 0:
                tileInformation->_targetMaterial->setDiffuse(iaColor3f(0.7f, 0.7f, 0.7f));
                break;
            case 1:
                tileInformation->_targetMaterial->setDiffuse(iaColor3f(0.7f, 0.0f, 0.0f));
                break;
            case 2:
                tileInformation->_targetMaterial->setDiffuse(iaColor3f(0.0f, 0.7f, 0.0f));
                break;
            case 3:
                tileInformation->_targetMaterial->setDiffuse(iaColor3f(0.0f, 0.0f, 0.7f));
                break;
            case 4:
                tileInformation->_targetMaterial->setDiffuse(iaColor3f(0.7f, 0.0f, 0.7f));
                break;
            case 5:
                tileInformation->_targetMaterial->setDiffuse(iaColor3f(0.7f, 0.7f, 0.0f));
                break;
            case 6:
                tileInformation->_targetMaterial->setDiffuse(iaColor3f(0.0f, 0.7f, 0.7f));
                break;
            case 7:
                tileInformation->_targetMaterial->setDiffuse(iaColor3f(0.3f, 0.3f, 0.3f));
                break;
            }
#endif

            meshNode->setTargetMaterial(tileInformation->_targetMaterial);
            result->insertNode(meshNode);

#ifndef DEBUG_VOXEL_TERRAIN_NO_PHYSICS
            if (tileInformation->_lod == 0)
            {
                iNodePhysics* physicsNode = static_cast<iNodePhysics*>(iNodeFactory::getInstance().createNode(iNodeType::iNodePhysics));
                iaMatrixd offset;
                physicsNode->addMesh(mesh, 1, offset);
                physicsNode->finalizeCollision(true);
                physicsNode->setMaterial(tileInformation->_physicsMaterialID);

                result->insertNode(physicsNode);
            }
#endif
        }

        delete voxelData;
        delete voxelDataNextLOD;

        return result;
    }
    __IGOR_ENABLE_WARNING__(4100)

}
