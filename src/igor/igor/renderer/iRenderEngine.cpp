// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/iRenderEngine.h>

#include <igor/renderer/iRenderer.h>
#include <igor/entities/components/iTransformComponent.h>
#include <igor/entities/components/iCameraComponent.h>
#include <igor/entities/components/iMeshRenderComponent.h>

namespace igor
{

    void iRenderEngine::setScene(iEntityScenePtr scene)
    {
        _scene = scene;
    }

    void iRenderEngine::setCamera(iEntityPtr camera)
    {
        con_assert(camera != nullptr, "zero pointer");
        con_assert(camera->getScene() == _scene, "incompatible scene");

        auto cameraComponent = camera->getComponent<iCameraComponent>();
        auto transform = camera->getComponent<iTransformComponent>();
        con_assert(cameraComponent != nullptr && transform != nullptr, "missing components");

        _cameraID = camera->getID();
    }

    void iRenderEngine::addMesh(iEntityPtr meshEntity)
    {
        con_assert(meshEntity != nullptr, "zero pointer");
        con_assert(meshEntity->getScene() == _scene, "incompatible scene");

        auto transformComponent = meshEntity->getComponent<iTransformComponent>();
        auto meshRenderComponent = meshEntity->getComponent<iMeshRenderComponent>();
        con_assert(meshRenderComponent != nullptr && transformComponent != nullptr, "missing components");

        iMaterialPtr material = meshRenderComponent->getMaterial();
        iShaderPtr shader = material->getShader();
        if (!shader->isValid() ||
            shader->getRenderState(iRenderState::Instanced) == iRenderStateValue::Off) // only instanced is supported for now
        {
            return;
        }

        iaMatrixd src = transformComponent->getWorldMatrix();
        iaMatrixf dst;
        for (int i = 0; i < 16; ++i)
        {
            dst[i] = src[i];
        }

        auto mesh = meshRenderComponent->getMesh();

        auto iter = std::find_if(_materialGroups.begin(), _materialGroups.end(),
                                 [&shader](const iMaterialGroup &materialGroup)
                                 { return materialGroup._shader == shader; });

        if (iter == _materialGroups.end())
        {
            std::unordered_map<iMeshPtr, iInstaningPackage> instancing;
            instancing[mesh]._buffer = iInstancingBuffer::create(std::vector<iBufferLayoutEntry>{{iShaderDataType::Matrix4x4}}, 10000);
            instancing[mesh]._buffer->addInstance(sizeof(iaMatrixf), dst.getData());
            instancing[mesh]._material = material;
            _materialGroups.push_back({shader, instancing});
        }
        else
        {
            iter->_instancing[mesh]._buffer->addInstance(sizeof(iaMatrixf), dst.getData());
        }
    }

    void iRenderEngine::setupCamera(iEntityPtr camera, const iaRectanglei &viewport)
    {
        auto cameraComponent = camera->getComponent<iCameraComponent>();
        auto transformComponent = camera->getComponent<iTransformComponent>();
        const auto &camViewport = cameraComponent->getViewport();
        const auto &camWorldMatrix = transformComponent->_worldMatrix;

        iaRectanglei rect;
        rect.setX(viewport.getX() + camViewport.getX() * static_cast<float32>(viewport.getWidth()) + 0.5f);
        rect.setY(viewport.getY() + camViewport.getY() * static_cast<float32>(viewport.getHeight()) + 0.5f);
        rect.setWidth(camViewport.getWidth() * static_cast<float32>(viewport.getWidth()) + 0.5f);
        rect.setHeight(camViewport.getHeight() * static_cast<float32>(viewport.getHeight()) + 0.5f);
        iRenderer::getInstance().setViewport(rect);

        if (cameraComponent->isClearColorActive())
        {
            iRenderer::getInstance().clearColorBuffer(cameraComponent->getClearColor());
        }

        if (cameraComponent->isClearDepthActive())
        {
            iRenderer::getInstance().clearDepthBuffer(cameraComponent->getClearDepth());
        }

        if (cameraComponent->getProjectionType() == iProjectionType::Perspective)
        {
            iRenderer::getInstance().setPerspective(cameraComponent->getFieldOfView(),
                                                    cameraComponent->getAspectRatio(),
                                                    cameraComponent->getNearClipPlane(),
                                                    cameraComponent->getFarClipPlane());
        }
        else
        {
            iRenderer::getInstance().setOrtho(cameraComponent->getLeftOrtho(),
                                              cameraComponent->getRightOrtho(),
                                              cameraComponent->getBottomOrtho(),
                                              cameraComponent->getTopOrtho(),
                                              cameraComponent->getNearClipPlane(),
                                              cameraComponent->getFarClipPlane());
        }

        iRenderer::getInstance().setViewMatrixFromCam(camWorldMatrix);

        iaMatrixd viewmatrix;
        viewmatrix.lookAt(camWorldMatrix._pos, camWorldMatrix._pos - camWorldMatrix._depth, camWorldMatrix._top);

        iaMatrixd projectionViewMatrix = iRenderer::getInstance().getProjectionMatrix();
        projectionViewMatrix *= viewmatrix;
        _frustum.set(projectionViewMatrix);
    }

    void iRenderEngine::render(const iaRectanglei &viewport)
    {
        auto camera = _scene->getEntity(_cameraID);
        if (camera == nullptr)
        {
            return;
        }

        setupCamera(camera, viewport);

        std::sort(_materialGroups.begin(), _materialGroups.end(), [](const iMaterialGroup a, const iMaterialGroup b) -> bool
                  { return a._shader->getOrder() < b._shader->getOrder(); });

        for (const auto &materialGroup : _materialGroups)
        {
            for (const auto &pair : materialGroup._instancing)
            {
                iRenderer::getInstance().drawMeshInstanced(pair.first, pair.second._buffer, pair.second._material);
                pair.second._buffer->clear();
            }
        }
    }

    const iFrustumd &iRenderEngine::getFrustum() const
    {
        return _frustum;
    }

} // namespace igor