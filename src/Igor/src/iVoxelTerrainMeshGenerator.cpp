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

            iTargetMaterial* targetMaterial = meshNode->getTargetMaterial();
#if 0
            targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("dirt.png"), 0);
            targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("grass.png"), 1);
            targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("rock.png"), 2);

            targetMaterial->setAmbient(iaColor3f(0.7f, 0.7f, 0.7f));
            targetMaterial->setDiffuse(iaColor3f(0.9f, 0.9f, 0.9f));
            targetMaterial->setSpecular(iaColor3f(0.1f, 0.1f, 0.1f));
            targetMaterial->setEmissive(iaColor3f(0.05f, 0.05f, 0.05f));
            targetMaterial->setShininess(100.0f);
#else

#if 1
            targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("white.png"), 0);
            targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("white.png"), 1);
            targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("white.png"), 2);

            iaRandomNumberGenerator rand;
#if 1
            rand.setSeed(reinterpret_cast<uint32>(voxelData));
#else
            rand.setSeed(tileInformation->_lod);
#endif

            float32 r = ((rand.getNext() % 70) + 15.0f) / 100.0f;
            float32 g = ((rand.getNext() % 70) + 15.0f) / 100.0f;
            float32 b = ((rand.getNext() % 70) + 15.0f) / 100.0f;

			iaColor3f ambient(r * 0.7f, g* 0.7f, b* 0.7f);
			iaColor3f diffuse(r * 0.9f, g* 0.9f, b* 0.9f);
			iaColor3f specular(r * 0.1f, g* 0.1f, b* 0.1f);
			iaColor3f emissive(r * 0.05f, g* 0.05f, b* 0.05f);

            targetMaterial->setAmbient(ambient);
            targetMaterial->setDiffuse(diffuse);
            targetMaterial->setSpecular(specular);
            targetMaterial->setEmissive(emissive);
            targetMaterial->setShininess(100.0f);

#else
            switch (tileInformation->_lod)
            {
            case 0:
                targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("gray.png"), 0);
                targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("gray.png"), 1);
                targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("gray.png"), 2);
                break;
            case 1:
                targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("green.png"), 0);
                targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("green.png"), 1);
                targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("green.png"), 2);
                break;
            case 2:
                targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("blue.png"), 0);
                targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("blue.png"), 1);
                targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("blue.png"), 2);
                break;
            case 3:
                targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("red.png"), 0);
                targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("red.png"), 1);
                targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("red.png"), 2);
                break;
            case 4:
                targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("yellow.png"), 0);
                targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("yellow.png"), 1);
                targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("yellow.png"), 2);
                break;
            case 5:
                targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("cyan.png"), 0);
                targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("cyan.png"), 1);
                targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("cyan.png"), 2);
                break;
            case 6:
                targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("magenta.png"), 0);
                targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("magenta.png"), 1);
                targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("magenta.png"), 2);
                break;
            case 7:
                targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("black.png"), 0);
                targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("black.png"), 1);
                targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("black.png"), 2);
                break;
            }

            targetMaterial->setAmbient(iaColor3f(0.7f, 0.7f, 0.7f));
            targetMaterial->setDiffuse(iaColor3f(0.9f, 0.9f, 0.9f));
            targetMaterial->setSpecular(iaColor3f(0.1f, 0.1f, 0.1f));
            targetMaterial->setEmissive(iaColor3f(0.05f, 0.05f, 0.05f));
            targetMaterial->setShininess(100.0f);
#endif
#endif

            result->insertNode(meshNode);

            /*if (tileInformation->_lod == 0)
            {
                iNodePhysics* physicsNode = static_cast<iNodePhysics*>(iNodeFactory::getInstance().createNode(iNodeType::iNodePhysics));
                iaMatrixd offset;
                physicsNode->addMesh(mesh, 1, offset);
                physicsNode->finalizeCollision(true);
                physicsNode->setMaterial(EntityManager::getInstance().getTerrainMaterialID());

                result->insertNode(physicsNode);
            }*/
        }

        delete voxelData;
        delete voxelDataNextLOD;

        return result;
    }
	__IGOR_ENABLE_WARNING__(4100)

}
