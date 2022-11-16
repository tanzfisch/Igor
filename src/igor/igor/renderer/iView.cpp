// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/iView.h>

#include <igor/system/iWindow.h>
#include <igor/renderer/iRenderer.h>
#include <igor/renderer/iRenderer2.h>
#include <igor/resources/texture/iTextureResourceFactory.h>
#include <igor/scene/iScene.h>
#include <igor/resources/profiler/iProfiler.h>

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
            con_warn("not all delegates unregistered from " << getName());
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
        _renderEngine.setWireframeVisible(wireframe);
    }

    bool iView::isWireframeVisible() const
    {
        return _renderEngine.isWireframeVisible();
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

    void iView::setViewport(iaRectanglef rect)
    {
        _viewportConfig = rect;
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

    void iView::setClipPlanes(float32 nearPlane, float32 farPlane)
    {
        _nearPlaneDistance = nearPlane;
        _farPlaneDistance = farPlane;
    }

    void iView::setClearColor(bool active)
    {
        _clearColorActive = active;
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

    void iView::setCurrentCamera(uint64 cameraID)
    {
        _renderEngine.setCurrentCamera(cameraID);
    }

    uint64 iView::getCurrentCamera() const
    {
        return _renderEngine.getCurrentCamera();
    }

    void iView::draw()
    {
        if (_scene != nullptr)
        {
            _scene->handle();
        }

        if (_visible)
        {
            iRenderer::getInstance().setViewport(_viewport.getX(), _viewport.getY(), _viewport.getWidth(), _viewport.getHeight());
            iRenderer2::getInstance().setViewport(_viewport.getX(), _viewport.getY(), _viewport.getWidth(), _viewport.getHeight());

            if (_clearColorActive)
            {
                iRenderer::getInstance().setClearColor(_clearColor);
                iRenderer::getInstance().clearColorBuffer();

                iRenderer2::getInstance().clearColorBuffer(_clearColor);
            }

            if (_clearDepthActive)
            {
                iRenderer::getInstance().setClearDepth(_clearDepth);
                iRenderer::getInstance().clearDepthBuffer();

                iRenderer2::getInstance().clearDepthBuffer(_clearDepth);
            }

            if (_perspective)
            {
                iRenderer::getInstance().setPerspective(_viewAngel, getAspectRatio(), _nearPlaneDistance, _farPlaneDistance);

                iRenderer2::getInstance().setPerspective(_viewAngel, getAspectRatio(), _nearPlaneDistance, _farPlaneDistance);
            }
            else
            {
                iRenderer::getInstance().setOrtho(_left, _right, _bottom, _top, _nearPlaneDistance, _farPlaneDistance);

                iRenderer2::getInstance().setOrtho(_left, _right, _bottom, _top, _nearPlaneDistance, _farPlaneDistance);
            }

            if (_scene != nullptr)
            {
                _renderEngine.render();
            }

            {
                IGOR_PROFILER_SCOPED(udraw);
                _renderEvent();
            }
        }

        iRenderer2::getInstance().endFrame();
    }

    uint64 iView::pickcolorID(uint32 posx, uint32 posy)
    {
        std::vector<uint64> colorIDs;

        pickcolorID(iaRectanglei(posx, posy, 1, 1), colorIDs);

        return colorIDs.front();
    }

    // TODO use alpha channel for color ID as well
    void iView::pickcolorID(const iaRectanglei &rectangle, std::vector<uint64> &colorIDs)
    {
        // TODO check ranges

        if (_scene != nullptr &&
            getCurrentCamera() != iNode::INVALID_NODE_ID)
        {
            iRenderEngine renderEngine;

            uint32 renderTarget = iRenderer::getInstance().createRenderTarget(_viewport.getWidth(), _viewport.getHeight(), iColorFormat::RGBA, iRenderTargetType::ToRenderBuffer, true);
            iRenderer::getInstance().setRenderTarget(renderTarget);

            iRenderer::getInstance().setViewport(0, 0, _viewport.getWidth(), _viewport.getHeight());

            iRenderer::getInstance().setClearColor(iaColor4f(0, 0, 0, 0));
            iRenderer::getInstance().setClearDepth(1.0);

            iRenderer::getInstance().clearColorBuffer();
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
            renderEngine.setCurrentCamera(getCurrentCamera());
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
        _scene = scene;
        _renderEngine.setScene(_scene);
    }

    iScenePtr iView::getScene()
    {
        return _scene;
    }

    void iView::registerRenderDelegate(iDrawDelegate render_delegate)
    {
        _renderEvent.append(render_delegate);
    }

    void iView::unregisterRenderDelegate(iDrawDelegate render_delegate)
    {
        _renderEvent.remove(render_delegate);
    }

    float32 iView::getAspectRatio() const
    {
        return static_cast<float32>(_viewport.getWidth()) / static_cast<float32>(_viewport.getHeight());
    }

    iaVector3d iView::project(const iaVector3d &worldSpacePos, const iaMatrixd &cameraMatrix)
    {
        iaMatrixd viewMatrix;
        viewMatrix.lookAt(cameraMatrix._pos, cameraMatrix._pos - cameraMatrix._depth, cameraMatrix._top);

        iaMatrixd projectionMatrix;
        projectionMatrix.perspective(_viewAngel, getAspectRatio(), _nearPlaneDistance, _farPlaneDistance);

        return iRenderer2::getInstance().project(worldSpacePos, viewMatrix, projectionMatrix, _viewport);
    }

    iaVector3d iView::unProject(const iaVector3d &screenpos, const iaMatrixd &cameraMatrix)
    {
        iaMatrixd viewMatrix;
        viewMatrix.lookAt(cameraMatrix._pos, cameraMatrix._pos - cameraMatrix._depth, cameraMatrix._top);

        iaMatrixd modelViewMatrix = viewMatrix;
        modelViewMatrix *= cameraMatrix;

        iaMatrixd projectionMatrix;
        projectionMatrix.perspective(_viewAngel, getAspectRatio(), _nearPlaneDistance, _farPlaneDistance);

        return iRenderer2::getInstance().unProject(screenpos, modelViewMatrix, projectionMatrix, _viewport);
    }

    /*	iPixmap* iView::makeScreenshot(bool alphachannel)
    {
    iPixmap *pixmap = 0;

    if(alphachannel) //! \todo
    {
    pixmap = new iPixmap(dcWindow.getWidth()*width, dcWindow.getHeight()*height, CF_RGBA);
    dcRenderer.readPixels(0, 0, pixmap->getWidth(), pixmap->getHeight(), CF_RGBA, pixmap->getData());
    }
    else
    {
    pixmap = new iPixmap(dcWindow.getWidth()*width, dcWindow.getHeight()*height, CF_RGB);
    dcRenderer.readPixels(0, 0, pixmap->getWidth(), pixmap->getHeight(), CF_RGB, pixmap->getData());
    }

    return pixmap;
    }*/

}; // namespace igor
