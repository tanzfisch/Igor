// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "VoxelTerrainMeshGenerator.h"

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
#include <iNodePhysicsMesh.h>
using namespace Igor;

VoxelTerrainMeshGenerator::VoxelTerrainMeshGenerator()
{
    _identifier = "vtg";
    _name = "Voxel Terrain Generator";
}

iModelDataIO* VoxelTerrainMeshGenerator::createInstance()
{
    VoxelTerrainMeshGenerator* result = new VoxelTerrainMeshGenerator();
    return static_cast<iModelDataIO*>(result);
}

iNode* VoxelTerrainMeshGenerator::importData(const iaString& sectionName, iModelDataInputParameter* parameter)
{
    TileInformation* tileInformation = reinterpret_cast<TileInformation*>(parameter->_parameters.getDataPointer());
    const iaVector3I& relativePos = tileInformation->_relativePos;
    const iaVector3I& absolutePos = tileInformation->_absolutePos;
    int64 width = tileInformation->_width;
    int64 depth = tileInformation->_depth;
    int64 height = tileInformation->_height;
    iVoxelData* voxelData = tileInformation->_voxelData;

    if (relativePos._x + width >= voxelData->getWidth())
    {
        width = voxelData->getWidth() - 1 - relativePos._x;
    }

    if (relativePos._z + depth >= voxelData->getDepth())
    {
        depth = voxelData->getDepth() - 1 - relativePos._z;
    }

    if (relativePos._y + height >= voxelData->getHeight())
    {
        height = voxelData->getHeight() - 1 - relativePos._y;
    }

    iNode* result = iNodeFactory::getInstance().createNode(iNodeType::iNode);

    iContouringCubes contouringCubes;
    contouringCubes.setVoxelData(voxelData);

    shared_ptr<iMesh> mesh = contouringCubes.compile(relativePos, iaVector3I(width, height, depth));

    if (mesh.get() != nullptr)
    {
        iNodeMesh* meshNode = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
        meshNode->setMesh(mesh);
        meshNode->setMaterial(tileInformation->_materialID);

        iTargetMaterial* targetMaterial = meshNode->getTargetMaterial();
        targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("dirt.png"), 0);
        targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("grass.png"), 1);
        targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("rock.png"), 2);
        targetMaterial->setAmbient(iaColor3f(0.5f, 0.5f, 0.5f));
        targetMaterial->setDiffuse(iaColor3f(0.7f, 0.7f, 0.7f));
        targetMaterial->setSpecular(iaColor3f(0.1f, 0.1f, 0.1f));
        targetMaterial->setEmissive(iaColor3f(0.01f, 0.01f, 0.01f));
        targetMaterial->setShininess(100.0f);

        result->insertNode(meshNode);

        iNodePhysicsMesh* physicsMeshNode = static_cast<iNodePhysicsMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodePhysicsMesh));
        iaMatrixf offset;
        offset.translate(absolutePos._x, absolutePos._y, absolutePos._z);
        physicsMeshNode->setMesh(mesh, 1, offset);

        result->insertNode(physicsMeshNode);
    }

    delete parameter;

    return result;
}
