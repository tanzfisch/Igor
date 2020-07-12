// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "VoxelTerrainMeshGenerator.h"

VoxelTerrainMeshGenerator::VoxelTerrainMeshGenerator()
{
    _identifier = "vtg";
    _name = "Voxel Terrain Generator";
}

iModelDataIO *VoxelTerrainMeshGenerator::createInstance()
{
    VoxelTerrainMeshGenerator *result = new VoxelTerrainMeshGenerator();
    return static_cast<iModelDataIO *>(result);
}

iNodePtr VoxelTerrainMeshGenerator::importData(const iaString &sectionName, iModelDataInputParameter *parameter)
{
    TileInformation *tileInformation = reinterpret_cast<TileInformation *>(parameter->_parameters.getDataPointer());
    iVoxelData *voxelData = tileInformation->_voxelData;
    int64 width = voxelData->getWidth() - 1;
    int64 depth = voxelData->getDepth() - 1;
    int64 height = voxelData->getHeight() - 1;

    iNodePtr result = iNodeManager::getInstance().createNode<iNode>();

    iContouringCubes contouringCubes;
    contouringCubes.setVoxelData(voxelData);
    iMeshPtr mesh = contouringCubes.compile(iaVector3I(), iaVector3I(width, height, depth));

    if (mesh.get() != nullptr)
    {
        iNodeMesh *meshNode = iNodeManager::getInstance().createNode<iNodeMesh>();
        meshNode->setKeepMesh(parameter->_keepMesh);
        meshNode->setMesh(mesh);
        meshNode->setMaterial(tileInformation->_materialID);

        iTargetMaterial *targetMaterial = meshNode->getTargetMaterial();
        targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("grass.png"), 0);
        targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("dirt.png"), 1);
        targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("rock.png"), 2);
        targetMaterial->setAmbient(iaColor3f(0.7f, 0.7f, 0.7f));
        targetMaterial->setDiffuse(iaColor3f(0.9f, 0.9f, 0.9f));
        targetMaterial->setSpecular(iaColor3f(0.1f, 0.1f, 0.1f));
        targetMaterial->setEmissive(iaColor3f(0.01f, 0.01f, 0.01f));
        targetMaterial->setShininess(100.0f);

        result->insertNode(meshNode);
    }

    return result;
}
