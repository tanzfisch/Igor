// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/material/iMaterialGroup.h>

#include <igor/scene/nodes/iNodeRender.h>
#include <igor/scene/nodes/iNodeMesh.h>
#include <igor/resources/mesh/iMesh.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/resources/material/iMaterialResourceFactory.h>

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

    void iMaterialGroup::setMaterial(iMaterialPtr material)
    {
        _material = material;
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
        IGOR_PROFILER();

        for (auto &pair : _instancedRenderNodes)
        {
            pair.second._instancer->clearInstances();
        }

        _renderNodes.clear();
    }

    void iMaterialGroup::addRenderNode(iNodeRenderPtr renderNode)
    {
        IGOR_PROFILER();

        con_assert(_material != nullptr, "invalid material");

        const bool instancing = _material->getRenderState(iRenderState::Instanced) == iRenderStateValue::On;

        if (instancing &&
            iNodeType::iNodeMesh == renderNode->getType())
        {
            const auto meshBuffers = static_cast<iNodeMeshPtr>(renderNode)->getMeshBuffers();
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

            instanceData._instancer->addInstance(&matrix);
        }
        else
        {
            _renderNodes.push_back(renderNode);
        }
    } // namespace igor

} // namespace igor
