// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/renderer/iRenderEngine.h>

#include <igor/renderer/iRenderer.h>
#include <igor/entities/components/iTransformComponent.h>
#include <igor/entities/components/iCameraComponent.h>
#include <igor/entities/components/iMeshRenderComponent.h>
#include <igor/entities/iEntitySystemModule.h>
#include <igor/resources/iResourceManager.h>

#include <iaux/data/iaConvert.h>

namespace igor
{

    void iRenderEngine::setSceneID(const iEntitySceneID &sceneID)
    {
        _sceneID = sceneID;
    }

    void iRenderEngine::setCameraID(const iEntityID &cameraID)
    {
        _cameraID = cameraID;
    }

    void iRenderEngine::addMesh(iEntityPtr meshEntity)
    {
        con_assert(meshEntity != nullptr, "zero pointer");

        auto transformComponent = meshEntity->getComponent<iTransformComponent>();
        auto meshRenderComponent = meshEntity->getComponent<iMeshRenderComponent>();
        con_assert(meshRenderComponent != nullptr && transformComponent != nullptr, "missing components");

        const auto &meshReferences = meshRenderComponent->getMeshReferences();

        for (const auto &reference : meshReferences)
        {
            const auto mesh = reference._mesh;
            const auto material = reference._material;

            iShaderPtr shader = material->getShader();
            if (!shader->isValid())
            {
                continue;
            }

            auto iterMaterialGroup = std::find_if(_materialGroups.begin(), _materialGroups.end(),
                                                  [&shader](const iMaterialGroup &materialGroup)
                                                  { return materialGroup._shader == shader; });

            iMaterialGroup *materialGroup;

            if (iterMaterialGroup == _materialGroups.end())
            {
                iMaterialGroup group;
                group._shader = shader;
                _materialGroups.push_back(group);
                materialGroup = &_materialGroups.back();
            }
            else
            {
                materialGroup = &(*iterMaterialGroup);
            }

            if (shader->getRenderState(iRenderState::Instanced) == iRenderStateValue::Off)
            {
                materialGroup->_regular.emplace_back(mesh, transformComponent->getWorldMatrix(), material);
            }
            else
            {
                iaMatrixd src = transformComponent->getWorldMatrix();
                src *= reference._offset;
                iaMatrixf matrix;
                iaConvert::convert(src, matrix);

                if (materialGroup->_instancing[mesh]._buffer == nullptr)
                {
                    materialGroup->_instancing[mesh]._buffer = iInstancingBuffer::create(std::vector<iBufferLayoutEntry>{{iShaderDataType::Matrix4x4}});
                    materialGroup->_instancing[mesh]._material = material;
                }
                materialGroup->_instancing[mesh]._buffer->addInstance(sizeof(iaMatrixf), matrix.getData());
            }
        }
    }

    void iRenderEngine::renderMaterialGroups()
    {
        std::sort(_materialGroups.begin(), _materialGroups.end(), [](const iMaterialGroup a, const iMaterialGroup b) -> bool
                  { return a._shader->getOrder() < b._shader->getOrder(); });

        for (const auto &materialGroup : _materialGroups)
        {
            iRenderer::getInstance().setShader(materialGroup._shader);

            if (materialGroup._shader->getRenderState(iRenderState::Instanced) == iRenderStateValue::On)
            {
                for (const auto &pair : materialGroup._instancing)
                {
                    const iMeshPtr mesh = pair.first;
                    const iInstancingBufferPtr instancingBuffer = pair.second._buffer;
                    const iMaterialPtr material = pair.second._material;

                    iRenderer::getInstance().drawMeshInstanced(mesh, instancingBuffer, material);
                }
            }
            else
            {
                for(const auto &regular :  materialGroup._regular)
                {
                    iRenderer::getInstance().setModelMatrix(regular._matrix);
                    iRenderer::getInstance().drawMesh(regular._mesh, regular._material);
                }
            }
        }
    }

    void iRenderEngine::renderBoundingBoxes()
    {
        iaMatrixf srcMatrix;
        iaMatrixd dstMatrix;

        // TODO make the bbox write in to depth buffer
        for (const auto &materialGroup : _materialGroups)
        {
            for (const auto &pair : materialGroup._instancing)
            {
                const iMeshPtr mesh = pair.first;
                const iInstancingBufferPtr instancingBuffer = pair.second._buffer;

                for (int i = 0; i < instancingBuffer->getInstanceCount(); ++i)
                {
                    instancingBuffer->getInstance(i, sizeof(iaMatrixf), (void *)&srcMatrix);
                    iaConvert::convert(srcMatrix, dstMatrix);
                    iRenderer::getInstance().setModelMatrix(dstMatrix);

                    const iAABoxd &bbox = mesh->getBoundingBox();
                    iRenderer::getInstance().drawBox(bbox, iaColor4f(1.0, 1.0, 1.0, 0.5));
                }
            }
        }
    }

    void iRenderEngine::render()
    {
        renderMaterialGroups();

        if (_showBoundingBoxes)
        {
            renderBoundingBoxes();
        }

        // reset buffers ... TODO check how slow this is
        for (auto &materialGroup : _materialGroups)
        {
            materialGroup._regular.clear();
            for (const auto &pair : materialGroup._instancing)
            {
                pair.second._buffer->clear();
            }
        }
    }

    const iFrustumd &iRenderEngine::getFrustum() const
    {
        return _frustum;
    }

    void iRenderEngine::setFrustum(const iFrustumd &frustum)
    {
        _frustum = frustum;
    }

    void iRenderEngine::setFrustum(const iaMatrixd &matrix)
    {
        _frustum.set(matrix);
    }

    const iEntitySceneID &iRenderEngine::getSceneID() const
    {
        return _sceneID;
    }

    const iEntityID &iRenderEngine::getCameraID() const
    {
        return _cameraID;
    }

    void iRenderEngine::setBoundingBoxVisible(bool boundingBox)
    {
        _showBoundingBoxes = boundingBox;
    }

    bool iRenderEngine::isBoundingBoxVisible() const
    {
        return _showBoundingBoxes;
    }

} // namespace igor