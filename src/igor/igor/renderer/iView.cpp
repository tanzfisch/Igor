// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/iView.h>

#include <igor/system/iWindow.h>
#include <igor/renderer/iRenderer.h>
#include <igor/scene/iScene.h>
#include <igor/resources/profiler/iProfiler.h>
#include <igor/entities/iEntitySystemModule.h>

#include <iaux/math/iaRandomNumberGenerator.h>
#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <algorithm>

namespace igor
{

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
        _renderEngine.setOctreeVisible(octree);
    }

    bool iView::isOctreeVisible() const
    {
        return _renderEngine.isOctreeVisible();
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

    void iView::setCamera(iNodeID cameraID)
    {
        _renderEngine.setCamera(cameraID);
    }

    iNodeID iView::getCamera() const
    {
        return _renderEngine.getCamera();
    }

    void iView::setUpdateViewport(bool enabled)
    {
        _updateViewport = enabled;
    }

    bool iView::isUpdatingViewport() const
    {
        return _updateViewport;
    }

    void iView::draw()
    {
        if (_scene != nullptr)
        {
            _scene->handle();
        }

        if (!_visible)
        {
            return;
        }

        iRenderer::getInstance().setWireframeEnabled(_wireframeEnabled);        

        if(_entityScene != nullptr)
        {
            iEntitySystemModule::getInstance().onRender(_entityScene);
        }

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

        if (_scene != nullptr)
        {
            _renderEngine.render();
        }

        _renderEvent();
        
        iRenderer::getInstance().flush();
    }

    uint64 iView::pickColorID(const iaVector2i &pos)
    {
        return pickColorID(pos._x, pos._y);
    }

    uint64 iView::pickColorID(uint32 posx, uint32 posy)
    {
        std::vector<uint64> colorIDs;

        pickColorID(iaRectanglei(posx, posy, 1, 1), colorIDs);

        return colorIDs.front();
    }

    void iView::pickColorID(const iaRectanglei &rectangle, std::vector<uint64> &colorIDs)
    {
        if (_scene != nullptr &&
            getCamera() != iNode::INVALID_NODE_ID)
        {
            iRenderEngine renderEngine;

            uint32 renderTarget = iRenderer::getInstance().createRenderTarget(_viewport.getWidth(), _viewport.getHeight(), iColorFormat::RGBA, iRenderTargetType::ToRenderBuffer, true);
            iRenderer::getInstance().setRenderTarget(renderTarget);

            iRenderer::getInstance().setViewport(0, 0, _viewport.getWidth(), _viewport.getHeight());

            iRenderer::getInstance().clearColorBuffer(iaColor4f(0, 0, 0, 0));
            iRenderer::getInstance().clearDepthBuffer();

            if (_perspective)
            {
                iRenderer::getInstance().setPerspective(_viewAngel, getAspectRatio(), _nearPlaneDistance, _farPlaneDistance);
            }
            else
            {
                iRenderer::getInstance().setOrtho(_left, _right, _bottom, _top, _nearPlaneDistance, _farPlaneDistance);
            }

            renderEngine.setScene(_scene);
            renderEngine.setCamera(getCamera());
            _scene->handle();

            renderEngine.setColorIDRendering();
            renderEngine.render();

            int32 pixelCount = rectangle._width * rectangle._height;
            uint8 *data = new uint8[pixelCount * 4];
            iRenderer::getInstance().readPixels(rectangle._x, _viewport.getHeight() - rectangle._y, rectangle._width, rectangle._height, iColorFormat::RGBA, data);

            iRenderer::getInstance().setRenderTarget();
            iRenderer::getInstance().destroyRenderTarget(renderTarget);

            uint8 *dataIter = data;
            for (int i = 0; i < pixelCount; ++i)
            {
                uint64 colorID = (static_cast<uint64>(dataIter[0]) << 16) | (static_cast<uint64>(dataIter[1]) << 8) | (static_cast<uint64>(dataIter[2]));
                colorIDs.push_back(colorID);
                dataIter += 4;
            }

            delete[] data;
        }
    }

    void iView::updateWindowRect(const iaRectanglei &windowRect)
    {
        _windowRect = windowRect;

        _viewport.setX(_viewportConfig.getX() * static_cast<float32>(_windowRect.getWidth()) + 0.5f);
        _viewport.setY(_viewportConfig.getY() * static_cast<float32>(_windowRect.getHeight()) + 0.5f);
        _viewport.setWidth(_viewportConfig.getWidth() * static_cast<float32>(_windowRect.getWidth()) + 0.5f);
        _viewport.setHeight(_viewportConfig.getHeight() * static_cast<float32>(_windowRect.getHeight()) + 0.5f);
    }

    void iView::setScene(iScenePtr scene)
    {
        con_assert(scene != nullptr, "zero pointer");

        _scene = scene;
        _renderEngine.setScene(_scene);
    }

    iScenePtr iView::getScene() const
    {
        return _scene;
    }

    void iView::setEntityScene(iEntityScenePtr entityScene)
    {
        _entityScene = entityScene;
        _entityScene->setRenderEngine(&_renderEngine);
    }

    iEntityScenePtr iView::getEntityScene() const
    {
        return _entityScene;
    }

    void iView::registerRenderDelegate(iDrawDelegate render_delegate)
    {
        _renderEvent.add(render_delegate);
    }

    void iView::unregisterRenderDelegate(iDrawDelegate render_delegate)
    {
        _renderEvent.remove(render_delegate);
    }

    float64 iView::getAspectRatio() const
    {
        return static_cast<float64>(_viewport.getWidth()) / static_cast<float64>(_viewport.getHeight());
    }

    iaVector3d iView::project(const iaVector3d &worldSpacePos, const iaMatrixd &cameraMatrix)
    {
        iaMatrixd viewMatrix;
        viewMatrix.lookAt(cameraMatrix._pos, cameraMatrix._pos - cameraMatrix._depth, cameraMatrix._top);

        iaMatrixd projectionMatrix;
        projectionMatrix.perspective(_viewAngel, getAspectRatio(), _nearPlaneDistance, _farPlaneDistance);

        return iRenderer::getInstance().project(worldSpacePos, viewMatrix, projectionMatrix, _viewport);
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

}; // namespace igor
