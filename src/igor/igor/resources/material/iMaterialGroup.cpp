// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/material/iMaterialGroup.h>

#include <igor/scene/nodes/iNodeRender.h>
#include <igor/scene/nodes/iNodeMesh.h>
// #include <igor/resources/mesh/iMesh.h>

namespace igor
{
    iMaterialGroup::~iMaterialGroup()
    {
        for (auto &iter : _instancedRenderNodes)
        {
            if (iter.second._instancer != nullptr)
            {
                delete iter.second._instancer;
            }
        }
    }

    bool iMaterialGroup::isInstanced() const
    {
        return false; // TODO
    }

    bool iMaterialGroup::hasNodes() const
    {
        return !_renderNodes.empty() || !_instancedRenderNodes.empty();
    }

    const std::vector<iNodeRenderPtr> &iMaterialGroup::getRenderNodes() const
    {
        return _renderNodes;
    }

    const std::unordered_map<iMeshBuffersPtr, iInstancingData> &iMaterialGroup::getInstancedRenderNodes() const
    {
        return _instancedRenderNodes;
    }

    void iMaterialGroup::clear()
    {
        for (auto &pair : _instancedRenderNodes)
        {
            pair.second._instancer->clearInstances();
        }

        _renderNodes.clear();
    }

    void iMaterialGroup::addRenderNode(iNodeRenderPtr renderNode)
    {
        if (isInstanced() &&
            iNodeType::iNodeMesh == renderNode->getType())
        {
        /*    const auto meshBuffers = static_cast<iNodeMeshPtr>(renderNode)->getMeshBuffers();
            auto &instanceData = _instancedRenderNodes[meshBuffers];

            if (instanceData._instancer == nullptr)
            {
                instanceData._instancer = new iInstancer(64, 100000); // TODO ... later

                iNodeMeshPtr meshNode = static_cast<iNodeMeshPtr>(renderNode);
                instanceData._targetMaterial = meshNode->getTargetMaterial();
            }

            iaMatrixf matrix;
            const auto &worldMatrix = renderNode->getWorldMatrix();
            for (int i = 0; i < 16; ++i)
            {
                matrix[i] = worldMatrix[i];
            }

            instanceData._instancer->addInstance(&matrix);*/
        }
        else
        {
            _renderNodes.push_back(renderNode);
        }
    }

} // namespace igor
