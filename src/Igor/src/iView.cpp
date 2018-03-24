// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iView.h>

#include <iWindow.h>
#include <iRenderer.h>
#include <iTextureResourceFactory.h>
#include <iScene.h>
#include <iOctree.h>
#include <iStatistics.h>

#include <iaRandomNumberGenerator.h>
#include <iaConsole.h>
using namespace IgorAux;

#include <algorithm>
using namespace std;

namespace Igor
{

    iView::iView()
    {
#ifdef USE_VERBOSE_STATISTICS
        _userDrawSectionID = iStatistics::getInstance().registerSection("view:user", 2);
#endif
    }

    iView::~iView()
    {
#ifdef USE_VERBOSE_STATISTICS
        iStatistics::getInstance().unregisterSection(_userDrawSectionID);
#endif

        if (_renderEvent.hasDelegates())
        {
            con_warn("not all delegates unregistered");
            _renderEvent.clear();
        }
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

    void iView::setName(const iaString& name)
    {
        _name = name;
    }

    const iaString& iView::getName() const
    {
        return _name;
    }

    void iView::setClearDepth(bool active)
    {
        _clearDepthActive = active;
    }

    void iView::setClearDepth(float32 depth)
    {
        _clearDepth = depth;
    }

    void iView::setViewport(iRectanglef rect)
    {
        _viewportConfig = rect;
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

    void iView::setClearColor(iaColor4f& color)
    {
        _clearColor = color;
    }

    void iView::setClearColor(float32 r, float32 g, float32 b, float32 a)
    {
        _clearColor.set(r, g, b, a);
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

            if (_clearColorActive)
            {
                iRenderer::getInstance().setClearColor(_clearColor);
                iRenderer::getInstance().clearColorBuffer();
            }

            if (_clearDepthActive)
            {
                iRenderer::getInstance().setClearDepth(_clearDepth);
                iRenderer::getInstance().clearDepthBuffer();
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

#ifdef USE_VERBOSE_STATISTICS
            iStatistics::getInstance().beginSection(_userDrawSectionID);
#endif
            _renderEvent();

#ifdef USE_VERBOSE_STATISTICS
            iStatistics::getInstance().endSection(_userDrawSectionID);
#endif
        }
    }

    uint64 iView::pickcolorID(uint32 posx, uint32 posy)
    {
        vector<uint64> colorIDs;
        
        pickcolorID(iRectanglei(posx, posy, 1, 1), colorIDs);

        return colorIDs.front();
    }

    // TODO use alpha channel for color ID as well
    void iView::pickcolorID(const iRectanglei& rectangle, vector<uint64>& colorIDs)
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

            uint32 pixelCount = rectangle._width*rectangle._height;
            uint8* data = new uint8[pixelCount * 4];
            iRenderer::getInstance().readPixels(rectangle._x, _viewport.getHeight() - rectangle._y, rectangle._width, rectangle._height, iColorFormat::RGBA, data);

            iRenderer::getInstance().setRenderTarget();
            iRenderer::getInstance().destroyRenderTarget(renderTarget);

            uint8* dataIter = data;
            for (int i = 0; i < pixelCount; ++i)
            {
                uint64 colorID = (static_cast<uint64>(dataIter[0]) << 16) | (static_cast<uint64>(dataIter[1]) << 8) | (static_cast<uint64>(dataIter[2]));
                colorIDs.push_back(colorID);
                dataIter+=4;
            }

            delete[] data;
        }
    }

    void iView::updateWindowRect(const iRectanglei& windowRect)
    {
        _windowRect = windowRect;

        _viewport.setX(_viewportConfig.getX() * _windowRect.getWidth() + 0.5f);
        _viewport.setY(_viewportConfig.getY() * _windowRect.getHeight() + 0.5f);
        _viewport.setWidth(_viewportConfig.getWidth() * _windowRect.getWidth() + 0.5f);
        _viewport.setHeight(_viewportConfig.getHeight() * _windowRect.getHeight() + 0.5f);
    }

    void iView::setScene(iScene* scene)
    {
        _scene = scene;
        _renderEngine.setScene(_scene);
    }

    iScene* iView::getScene()
    {
        return _scene;
    }

    void iView::registerRenderDelegate(RenderDelegate render_delegate)
    {
        _renderEvent.append(render_delegate);
    }

    void iView::unregisterRenderDelegate(RenderDelegate render_delegate)
    {
        _renderEvent.remove(render_delegate);
    }

    float32 iView::getAspectRatio() const
    {
        return static_cast<float32>(_viewport.getWidth()) / static_cast<float32>(_viewport.getHeight());
    }

    iaVector3d iView::unProject(const iaVector3d& screenpos, const iaMatrixd& modelMatrix)
    {
        iaMatrixd viewMatrix;
        viewMatrix.lookAt(modelMatrix._pos, modelMatrix._pos - modelMatrix._depth, modelMatrix._top);

        iaMatrixd modelViewMatrix = viewMatrix;
        modelViewMatrix *= modelMatrix;

        iaMatrixd projectionMatrix;
        projectionMatrix.perspective(_viewAngel, getAspectRatio(), _nearPlaneDistance, _farPlaneDistance);

        return iRenderer::getInstance().unProject(screenpos, modelViewMatrix, projectionMatrix, _viewport);
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

};
