// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/traversal/iEntityColorIDTraverser.h>

#include <igor/entities/components/iTransformComponent.h>
#include <igor/entities/components/iMeshRenderComponent.h>
#include <igor/renderer/iRenderer.h>

namespace igor
{
    void iEntityColorIDTraverser::preTraverse()
    {
        // store current render states
        _viewport = iRenderer::getInstance().getViewport();
        _modelMatrix = iRenderer::getInstance().getModelMatrix();
        _viewMatrix = iRenderer::getInstance().getViewMatrix();
        _projectionMatrix = iRenderer::getInstance().getProjectionMatrix();
        _wireframeEnabled = iRenderer::getInstance().isWireframeEnabled();

        iRenderer::getInstance().setShader(iRenderer::getInstance().getColorIDMaterial());
    }

    bool iEntityColorIDTraverser::preOrderVisit(iEntityPtr entity)
    {
        auto transformComponent = entity->getComponent<iTransformComponent>();
        auto meshRenderComponent = entity->getComponent<iMeshRenderComponent>();
        if (transformComponent == nullptr || meshRenderComponent == nullptr)
        {
            return true;
        }

        uint32 value = static_cast<uint32>(static_cast<uint64>(entity->getID()));
        _idMap[value] = entity->getID();

        iRenderer::getInstance().setColorID(value);

        for(const auto &meshRef : meshRenderComponent->getMeshReferences())
        {
            iaMatrixd matrix = transformComponent->getWorldMatrix();
            matrix *= meshRef._offset;

            iRenderer::getInstance().setModelMatrix(matrix);
            iRenderer::getInstance().drawMesh(meshRef._mesh, meshRef._material);
        }

        return true;
    }

    void iEntityColorIDTraverser::postOrderVisit(iEntityPtr entity)
    {
    }

    void iEntityColorIDTraverser::postTraverse()
    {
        // restore everything
        iRenderer::getInstance().setWireframeEnabled(_wireframeEnabled);
        iRenderer::getInstance().setModelMatrix(_modelMatrix);
        iRenderer::getInstance().setViewMatrix(_viewMatrix);
        iRenderer::getInstance().setProjectionMatrix(_projectionMatrix);
        iRenderer::getInstance().setViewport(_viewport);
    }

    iEntityID iEntityColorIDTraverser::getEntityID(uint32 colorID)
    {
        const auto iter = _idMap.find(colorID);
        return (iter != _idMap.end()) ? iter->second : iEntityID::getInvalid();
    }

} // namespace igor