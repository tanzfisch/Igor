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
        con_assert(meshEntity->getScene() != nullptr, "no scene");
        con_assert(meshEntity->getScene()->getID() == _sceneID, "incompatible scene");

        auto transformComponent = meshEntity->getComponent<iTransformComponent>();
        auto meshRenderComponent = meshEntity->getComponent<iMeshRenderComponent>();
        con_assert(meshRenderComponent != nullptr && transformComponent != nullptr, "missing components");

        const auto &meshReferences = meshRenderComponent->getMeshReferences();

        for (const auto &reference : meshReferences)
        {
            auto material = reference._material;

            iShaderPtr shader = material->getShader();
            if (!shader->isValid() ||
                shader->getRenderState(iRenderState::Instanced) == iRenderStateValue::Off) // only instanced is supported for now
            {
                continue;
            }

            iaMatrixd src = transformComponent->getWorldMatrix();
            src *= reference._offset;
            iaMatrixf dst;
            iaConvert::convert(src, dst);

            auto iter = std::find_if(_materialGroups.begin(), _materialGroups.end(),
                                     [&shader](const iMaterialGroup &materialGroup)
                                     { return materialGroup._shader == shader; });

            auto mesh = reference._mesh;

            if (iter == _materialGroups.end())
            {
                std::unordered_map<iMeshPtr, iInstaningPackage> instancing;
                instancing[mesh]._buffer = iInstancingBuffer::create(std::vector<iBufferLayoutEntry>{{iShaderDataType::Matrix4x4}});
                instancing[mesh]._buffer->addInstance(sizeof(iaMatrixf), dst.getData());
                instancing[mesh]._material = material;
                _materialGroups.push_back({shader, instancing});
            }
            else
            {
                if (iter->_instancing[mesh]._buffer == nullptr)
                {
                    iter->_instancing[mesh]._buffer = iInstancingBuffer::create(std::vector<iBufferLayoutEntry>{{iShaderDataType::Matrix4x4}});
                    iter->_instancing[mesh]._material = material;
                }
                iter->_instancing[mesh]._buffer->addInstance(sizeof(iaMatrixf), dst.getData());
            }
        }
    }

    void iRenderEngine::renderInstances()
    {
        std::sort(_materialGroups.begin(), _materialGroups.end(), [](const iMaterialGroup a, const iMaterialGroup b) -> bool
                  { return a._shader->getOrder() < b._shader->getOrder(); });

        for (const auto &materialGroup : _materialGroups)
        {
            for (const auto &pair : materialGroup._instancing)
            {
                const iMeshPtr mesh = pair.first;
                const iInstancingBufferPtr instancingBuffer = pair.second._buffer;
                const iMaterialPtr material = pair.second._material;

                iRenderer::getInstance().setShader(materialGroup._shader);
                iRenderer::getInstance().drawMeshInstanced(mesh, instancingBuffer, material);
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

                for(int i=0;i<instancingBuffer->getInstanceCount();++i)
                {
                    instancingBuffer->getInstance(i, sizeof(iaMatrixf), (void*)&srcMatrix);
                    iaConvert::convert(srcMatrix, dstMatrix);
                    iRenderer::getInstance().setModelMatrix(dstMatrix);

                    const iAABoxd &bbox = mesh->getBoundingBox();
                    iRenderer::getInstance().drawBox(bbox, iaColor4f(1.0,1.0,1.0,0.5));
                }
            }
        }
    }

    void iRenderEngine::render()
    {
        renderInstances();

        if (_showBoundingBoxes)
        {
            renderBoundingBoxes();
        }

        // reset instancing buffers ... 
        for (const auto &materialGroup : _materialGroups)
        {
            for (const auto &pair : materialGroup._instancing)
            {
                pair.second._buffer->clear();;
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