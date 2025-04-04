// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
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
    const iaString &sectionName = parameters.getParameter<iaString>(IGOR_RESOURCE_PARAM_ALIAS, "");
    iVoxelData *voxelData = parameters.getParameter<iVoxelData *>("voxelData", nullptr);
    const bool keepMesh = parameters.getParameter<bool>(IGOR_RESOURCE_PARAM_KEEP_MESH, false);
    iShaderPtr terrainShader = parameters.getParameter<iShaderPtr>(IGOR_RESOURCE_SHADER, nullptr);

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

        iParameters param({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
                           {IGOR_RESOURCE_PARAM_GENERATE, true},
                           {IGOR_RESOURCE_PARAM_SHADER, terrainShader},
                           {IGOR_RESOURCE_PARAM_TEXTURE0, iResourceManager::getInstance().requestResource<iTexture>("example_texture_grass")},
                           {IGOR_RESOURCE_PARAM_TEXTURE1, iResourceManager::getInstance().requestResource<iTexture>("example_texture_dirt")},
                           {IGOR_RESOURCE_PARAM_TEXTURE2, iResourceManager::getInstance().requestResource<iTexture>("example_texture_rock")},
                           {IGOR_RESOURCE_PARAM_AMBIENT, iaColor3f(0.7f, 0.7f, 0.7f)},
                           {IGOR_RESOURCE_PARAM_DIFFUSE, iaColor3f(0.9f, 0.9f, 0.9f)},
                           {IGOR_RESOURCE_PARAM_SPECULAR, iaColor3f(0.1f, 0.1f, 0.1f)},
                           {IGOR_RESOURCE_PARAM_EMISSIVE, iaColor3f(0.01f, 0.01f, 0.01f)},
                           {IGOR_RESOURCE_PARAM_SHININESS, 100.0f}});
        iMaterialPtr material = iResourceManager::getInstance().loadResource<iMaterial>(param);
        meshNode->setMaterial(material);
        result->insertNode(meshNode);
    }

    return result;
}
