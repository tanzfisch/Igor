// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/renderer/iView.h>

#include <igor/system/iTimer.h>
#include <igor/system/iWindow.h>
#include <igor/renderer/iRenderer.h>
#include <igor/renderer/environment/iSkyBox.h>
#include <igor/resources/profiler/iProfiler.h>
#include <igor/resources/iResourceManager.h>
#include <igor/entities/iEntitySystemModule.h>
#include <igor/entities/components/iTransformComponent.h>
#include <igor/entities/components/iCameraComponent.h>
#include <igor/entities/traversal/iEntityColorIDTraverser.h>

#include <iaux/math/iaRandomNumberGenerator.h>
#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <algorithm>

namespace igor
{

    iView::iView()
    {
        _skyBox = std::make_unique<iSkyBox>();
        _skyBox->setTexture(iResourceManager::getInstance().requestResource<iTexture>("igor_skybox_clearsky"));
    }

    iView::~iView()
    {
        if (_renderEvent.hasDelegates())
        {
            con_warn("not all delegates unregistered from view \"" << getName() << "\"");
            _renderEvent.clear();
        }
    }

    void iView::setZIndex(int32 zindex)
    {
        _zIndex = zindex;
    }

    int32 iView::getZIndex() const
    {
        return _zIndex;
    }

    void iView::setVisible(bool visible)
    {
        _visible = visible;
    }

    bool iView::isVisible() const
    {
        return _visible;
    }

    void iView::setWireframeVisible(bool wireframe)
    {
        _wireframeEnabled = wireframe;
    }

    bool iView::isWireframeVisible() const
    {
        return _wireframeEnabled;
    }

    void iView::setBoundingBoxVisible(bool boundingBox)
    {
        _renderEngine.setBoundingBoxVisible(boundingBox);
    }

    bool iView::isBoundingBoxVisible() const
    {
        return _renderEngine.isBoundingBoxVisible();
    }

    void iView::setOctreeVisible(bool octree)
    {
        // TODO        _renderEngineOld.setOctreeVisible(octree);
    }

    bool iView::isOctreeVisible() const
    {
        // TODO        return _renderEngineOld.isOctreeVisible();
        return false;
    }

    void iView::setName(const iaString &name)
    {
        _name = name;
    }

    const iaString &iView::getName() const
    {
        return _name;
    }

    void iView::setClearDepthActive(bool active)
    {
        _clearDepthActive = active;
    }

    bool iView::isClearDepthActive() const
    {
        return _clearDepthActive;
    }

    void iView::setClearDepth(float32 depth)
    {
        _clearDepth = depth;
    }

    float32 iView::getClearDepth() const
    {
        return _clearDepth;
    }

    void iView::setViewportRelative(iaRectanglef rect)
    {
        _viewportConfig = rect;
    }

    iaRectanglei iView::getViewport() const
    {
        return _viewport;
    }

    bool iView::isPerspective() const
    {
        return _perspective;
    }

    void iView::setPerspective(float32 viewAngel)
    {
        if (viewAngel > 0.0f && viewAngel < 180.0f)
        {
            _viewAngel = viewAngel;
            _perspective = true;
        }
        else
        {
            con_err("value out of range");
        }
    }

    void iView::setOrthogonal(float32 left, float32 right, float32 bottom, float32 top)
    {
        _left = left;
        _right = right;
        _top = top;
        _bottom = bottom;
        _perspective = false;
    }

    void iView::setClipPlanes(float32 nearPlain, float32 farPlain)
    {
        _nearPlaneDistance = nearPlain;
        _farPlaneDistance = farPlain;
    }

    void iView::setClearColorActive(bool active)
    {
        _clearColorActive = active;
    }

    bool iView::isClearColorActive() const
    {
        return _clearColorActive;
    }

    void iView::setClearColor(const iaColor4f &color)
    {
        _clearColor = color;
    }

    void iView::setClearColor(float32 r, float32 g, float32 b, float32 a)
    {
        _clearColor.set(r, g, b, a);
    }

    const iaColor4f &iView::getClearColor() const
    {
        return _clearColor;
    }

    void iView::setUpdateViewport(bool enabled)
    {
        _updateViewport = enabled;
    }

    bool iView::isUpdatingViewport() const
    {
        return _updateViewport;
    }

    void iView::setupCamera()
    {
        auto sceneID = _overrideSceneID.isValid() ? _overrideSceneID : _renderEngine.getSceneID();
        auto cameraID = _overrideCameraID.isValid() ? _overrideCameraID : _renderEngine.getCameraID();

        auto scene = iEntitySystemModule::getInstance().getScene(sceneID);
        if (scene == nullptr)
        {
            return;
        }

        iEntityPtr camera = scene->getEntity(cameraID);
        if (camera == nullptr)
        {
            return;
        }

        auto cameraComponent = camera->getComponent<iCameraComponent>();
        const auto &camViewport = cameraComponent->getViewport();

        auto transformComponent = camera->getComponent<iTransformComponent>();
        const auto &camWorldMatrix = transformComponent->getWorldMatrix();

        iaRectanglei rect;
        rect.setX(_viewport.getX() + camViewport.getX() * static_cast<float32>(_viewport.getWidth()) + 0.5f);
        rect.setY(_viewport.getY() + camViewport.getY() * static_cast<float32>(_viewport.getHeight()) + 0.5f);
        rect.setWidth(camViewport.getWidth() * static_cast<float32>(_viewport.getWidth()) + 0.5f);
        rect.setHeight(camViewport.getHeight() * static_cast<float32>(_viewport.getHeight()) + 0.5f);
        iRenderer::getInstance().setViewport(rect);

        iRenderer::getInstance().setWireframeEnabled(_wireframeEnabled);
        if (_clearColorActive && cameraComponent->isClearColorActive())
        {
            if (_embedded)
            {
                iRenderer::getInstance().setOrtho(0.0, 1.0, 1.0, 0.0, 0.001, 10.0);
                iRenderer::getInstance().drawFilledRectangle(0.0, 0.0, 1.0, 1.0, cameraComponent->getClearColor());
                iRenderer::getInstance().flush();
            }
            else
            {
                iRenderer::getInstance().clearColorBuffer(cameraComponent->getClearColor());
            }
        }

        if (_clearDepthActive && cameraComponent->isClearDepthActive())
        {
            iRenderer::getInstance().clearDepthBuffer(cameraComponent->getClearDepth());
        }

        if (cameraComponent->getProjectionType() == iProjectionType::Perspective)
        {
            float64 aspect = static_cast<float64>(rect.getWidth()) / static_cast<float64>(rect.getHeight());

            iRenderer::getInstance().setPerspective(cameraComponent->getFieldOfView(),
                                                    aspect,
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
        _renderEngine.setFrustum(projectionViewMatrix);
    }

    void iView::onRender()
    {
        if (!_visible)
        {
            return;
        }

        auto scene = iEntitySystemModule::getInstance().getScene(_entitySceneID);
        if (scene != nullptr)
        {
            // a system is setting viewport, perspective etc.
            iEntitySystemModule::getInstance().onPreRender(scene);
            setupCamera();
            _renderEngine.render();
            iEntitySystemModule::getInstance().onRender(scene);
        }
        else
        {
            iRenderer::getInstance().setWireframeEnabled(_wireframeEnabled);

            if (_updateViewport)
            {
                iRenderer::getInstance().setViewport(_viewport);
            }

            if (_clearColorActive)
            {
                iRenderer::getInstance().clearColorBuffer(_clearColor);
            }

            if (_clearDepthActive)
            {
                iRenderer::getInstance().clearDepthBuffer(_clearDepth);
            }

            if (_perspective)
            {
                iRenderer::getInstance().setPerspective(_viewAngel, getAspectRatio(), _nearPlaneDistance, _farPlaneDistance);
            }
            else
            {
                iRenderer::getInstance().setOrtho(_left, _right, _bottom, _top, _nearPlaneDistance, _farPlaneDistance);
            }
        }

        _renderEvent();
        iRenderer::getInstance().flush();
    }

    iEntityID iView::pickEntityID(const iaVector2i &pos)
    {
        return pickEntityID(pos._x, pos._y);
    }

    iEntityID iView::pickEntityID(uint32 posx, uint32 posy)
    {
        std::vector<iEntityID> colorIDs;

        pickEntityID(iaRectanglei(posx, posy, 1, 1), colorIDs);

        if (colorIDs.empty())
        {
            return 0;
        }

        return colorIDs.front();
    }

    void iView::pickEntityID(const iaRectanglei &rectangle, std::vector<iEntityID> &entityIDs)
    {
        auto scene = iEntitySystemModule::getInstance().getScene(_entitySceneID);
        if (scene == nullptr)
        {
            return;
        }

        auto camSceneID = _overrideSceneID.isValid() ? _overrideSceneID : _renderEngine.getSceneID();
        auto cameraID = _overrideCameraID.isValid() ? _overrideCameraID : _renderEngine.getCameraID();

        auto camScene = iEntitySystemModule::getInstance().getScene(camSceneID);
        if (camScene == nullptr)
        {
            return;
        }

        iEntityPtr camera = camScene->getEntity(cameraID);
        if (camera == nullptr)
        {
            return;
        }

        iRenderEngine tempRenderEngine;

        auto transformComponent = camera->getComponent<iTransformComponent>();
        const auto &camWorldMatrix = transformComponent->getWorldMatrix();

        const iRenderTargetID renderTarget = iRenderer::getInstance().createRenderTarget(_viewport.getWidth(), _viewport.getHeight(), iColorFormat::RGBA, iRenderTargetType::ToRenderBuffer, true);
        iRenderer::getInstance().setRenderTarget(renderTarget);

        iRenderer::getInstance().setViewport(0, 0, _viewport.getWidth(), _viewport.getHeight());

        iRenderer::getInstance().clearColorBuffer(iaColor4f(0.0f, 0.0f, 0.0f, 0.0f));
        iRenderer::getInstance().clearDepthBuffer();

        if (_perspective)
        {
            iRenderer::getInstance().setPerspective(_viewAngel, getAspectRatio(), _nearPlaneDistance, _farPlaneDistance);
        }
        else
        {
            iRenderer::getInstance().setOrtho(_left, _right, _bottom, _top, _nearPlaneDistance, _farPlaneDistance);
        }

        iRenderer::getInstance().setViewMatrixFromCam(camWorldMatrix);

        iaMatrixd viewmatrix;
        viewmatrix.lookAt(camWorldMatrix._pos, camWorldMatrix._pos - camWorldMatrix._depth, camWorldMatrix._top);

        iaMatrixd projectionViewMatrix = iRenderer::getInstance().getProjectionMatrix();
        projectionViewMatrix *= viewmatrix;
        _renderEngine.setFrustum(projectionViewMatrix);

        scene->setRenderEngine(&tempRenderEngine);

        iEntityColorIDTraverser traverser;
        traverser.traverse(scene);

        int32 pixelCount = rectangle._width * rectangle._height;
        uint8 *data = new uint8[pixelCount * 4];
        iRenderer::getInstance().readPixels(rectangle._x, _viewport.getHeight() - rectangle._y, rectangle._width, rectangle._height, iColorFormat::RGBA, data);

        iRenderer::getInstance().setRenderTarget();
        iRenderer::getInstance().destroyRenderTarget(renderTarget);

        uint8 *dataIter = data;
        for (int i = 0; i < pixelCount; ++i)
        {
            const uint32 colorID = (static_cast<uint32>(dataIter[0]) << 24) | (static_cast<uint32>(dataIter[1]) << 16) | (static_cast<uint32>(dataIter[2]) << 8) | (static_cast<uint32>(dataIter[3]));
            auto entityID = traverser.getEntityID(colorID);
            if (entityID.isValid())
            {
                entityIDs.push_back(entityID);
            }
            dataIter += 4;
        }

        delete[] data;

        scene->setRenderEngine(&_renderEngine);
    }

    void iView::updateWindowRect(const iaRectanglei &windowRect)
    {
        _windowRect = windowRect;

        _viewport.setX(_viewportConfig.getX() * static_cast<float32>(_windowRect.getWidth()) + 0.5f);
        _viewport.setY(_viewportConfig.getY() * static_cast<float32>(_windowRect.getHeight()) + 0.5f);
        _viewport.setWidth(_viewportConfig.getWidth() * static_cast<float32>(_windowRect.getWidth()) + 0.5f);
        _viewport.setHeight(_viewportConfig.getHeight() * static_cast<float32>(_windowRect.getHeight()) + 0.5f);
    }

    void iView::setScene(iEntityScenePtr entityScene)
    {
        if (entityScene != nullptr)
        {
            _entitySceneID = entityScene->getID();
            entityScene->setRenderEngine(&_renderEngine);
        }
    }

    void iView::setScene(const iEntitySceneID &entitySceneID)
    {
        setScene(iEntitySystemModule::getInstance().getScene(_entitySceneID));
    }

    const iEntitySceneID &iView::getSceneID() const
    {
        return _entitySceneID;
    }

    iRenderEvent &iView::getRenderEvent()
    {
        return _renderEvent;
    }

    float64 iView::getAspectRatio() const
    {
        return static_cast<float64>(_viewport.getWidth()) / static_cast<float64>(_viewport.getHeight());
    }

    iaVector3d iView::project(const iaVector3d &worldSpacePos)
    {
        auto sceneID = _overrideSceneID.isValid() ? _overrideSceneID : _renderEngine.getSceneID();
        auto cameraID = _overrideCameraID.isValid() ? _overrideCameraID : _renderEngine.getCameraID();

        auto scene = iEntitySystemModule::getInstance().getScene(sceneID);
        if (scene == nullptr)
        {
            con_err("no scene defined");
            return iaVector3d();
        }

        iEntityPtr camera = scene->getEntity(cameraID);
        if (camera == nullptr)
        {
            con_err("no camera found");
            return iaVector3d();
        }        

        auto camTransformComp = camera->getComponent<iTransformComponent>();
        if(camTransformComp == nullptr)
        {
            con_err("no transform component found");
            return iaVector3d();
        }

        return project(worldSpacePos, camTransformComp->getWorldMatrix());
    }

    iaVector3d iView::project(const iaVector3d &worldSpacePos, const iaMatrixd &cameraMatrix)
    {
        iaMatrixd viewMatrix;
        viewMatrix.lookAt(cameraMatrix._pos, cameraMatrix._pos - cameraMatrix._depth, cameraMatrix._top);

        iaMatrixd projectionMatrix;
        projectionMatrix.perspective(_viewAngel, getAspectRatio(), _nearPlaneDistance, _farPlaneDistance);

        return iRenderer::getInstance().project(worldSpacePos, viewMatrix, projectionMatrix, _viewport);
    }    

    iaVector3d iView::unProject(const iaVector3d &screenpos)
    {
        auto sceneID = _overrideSceneID.isValid() ? _overrideSceneID : _renderEngine.getSceneID();
        auto cameraID = _overrideCameraID.isValid() ? _overrideCameraID : _renderEngine.getCameraID();

        auto scene = iEntitySystemModule::getInstance().getScene(sceneID);
        if (scene == nullptr)
        {
            con_err("no scene defined");
            return iaVector3d();
        }

        iEntityPtr camera = scene->getEntity(cameraID);
        if (camera == nullptr)
        {
            con_err("no camera found");
            return iaVector3d();
        }        

        auto camTransformComp = camera->getComponent<iTransformComponent>();
        if(camTransformComp == nullptr)
        {
            con_err("no transform component found");
            return iaVector3d();
        }

        return unProject(screenpos, camTransformComp->getWorldMatrix());        
    }

    iaVector3d iView::unProject(const iaVector3d &screenpos, const iaMatrixd &cameraMatrix)
    {
        iaMatrixd viewMatrix;
        viewMatrix.lookAt(cameraMatrix._pos, cameraMatrix._pos - cameraMatrix._depth, cameraMatrix._top);

        iaMatrixd modelViewMatrix = viewMatrix;
        modelViewMatrix *= cameraMatrix;

        iaMatrixd projectionMatrix;
        projectionMatrix.perspective(_viewAngel, getAspectRatio(), _nearPlaneDistance, _farPlaneDistance);

        return iRenderer::getInstance().unProject(screenpos, modelViewMatrix, projectionMatrix, _viewport);
    }

    void iView::setEmbedded(bool embedded)
    {
        _embedded = embedded;
    }

    bool iView::isEmbedded()
    {
        return _embedded;
    }

    void iView::setOverrideCamera(iEntityPtr camera)
    {
        con_assert(camera != nullptr, "zero pointer");
        con_assert(camera->getComponent<iCameraComponent>() != nullptr, "no camera component");

        _overrideCameraID = camera->getID();
        _overrideSceneID = camera->getScene()->getID();
    }

    iEntityPtr iView::getOverrideCamera() const
    {
        auto scene = iEntitySystemModule::getInstance().getScene(_overrideSceneID);
        if (scene == nullptr)
        {
            return nullptr;
        }

        return scene->getEntity(_overrideCameraID);
    }

}; // namespace igor
