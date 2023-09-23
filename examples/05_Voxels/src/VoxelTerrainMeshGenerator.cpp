// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "VoxelTerrainMeshGenerator.h"

VoxelTerrainMeshGenerator::VoxelTerrainMeshGenerator()
{
    _identifier = "example.vtg";
    _name = "Voxel Terrain Generator";
}

iModelDataIO *VoxelTerrainMeshGenerator::createInstance()
{
    VoxelTerrainMeshGenerator *result = new VoxelTerrainMeshGenerator();
    return static_cast<iModelDataIO *>(result);
}

iNodePtr VoxelTerrainMeshGenerator::importData(const iParameters &parameters)
{
    const iaString &sectionName = parameters.getParameter<iaString>("name", "");
    iVoxelData *voxelData = parameters.getParameter<iVoxelData*>("voxelData", nullptr);
    const bool keepMesh = parameters.getParameter<bool>(IGOR_RESOURCE_PARAM_KEEP_MESH, false);
    iMaterialPtr material = parameters.getParameter<iMaterialPtr>(IGOR_RESOURCE_MATERIAL, nullptr);

    const int64 width = voxelData->getWidth() - 1;
    const int64 depth = voxelData->getDepth() - 1;
    const int64 height = voxelData->getHeight() - 1;

    iNodePtr result = iNodeManager::getInstance().createNode<iNode>();

    iContouringCubes contouringCubes;
    contouringCubes.setVoxelData(voxelData);
    iMeshPtr mesh = contouringCubes.compile(iaVector3I(), iaVector3I(width, height, depth));

    if (mesh.get() != nullptr)
    {
        iNodeMesh *meshNode = iNodeManager::getInstance().createNode<iNodeMesh>();
        mesh->setKeepRawData(keepMesh);
        meshNode->setMesh(mesh);
        meshNode->setMaterial(material);

        iTargetMaterialPtr targetMaterial = meshNode->getTargetMaterial();
        targetMaterial->setTexture(iResourceManager::getInstance().requestResource<iTexture>("example_texture_grass"), 0);
        targetMaterial->setTexture(iResourceManager::getInstance().requestResource<iTexture>("example_texture_dirt"), 1);
        targetMaterial->setTexture(iResourceManager::getInstance().requestResource<iTexture>("example_texture_rock"), 2);
        targetMaterial->setAmbient(iaColor3f(0.7f, 0.7f, 0.7f));
        targetMaterial->setDiffuse(iaColor3f(0.9f, 0.9f, 0.9f));
        targetMaterial->setSpecular(iaColor3f(0.1f, 0.1f, 0.1f));
        targetMaterial->setEmissive(iaColor3f(0.01f, 0.01f, 0.01f));
        targetMaterial->setShininess(100.0f);

        result->insertNode(meshNode);
    }

    return result;
}
