// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iView.h>

#include <iWindow.h>
#include <iRenderer.h>
#include <iTextureResourceFactory.h>
#include <iScene.h>
#include <iOctree.h>
#include <iStatistics.h>

#include <iaConsole.h>
using namespace IgorAux;

#include <algorithm>
using namespace std;

namespace Igor
{

    iView::iView()
    {
        _scenePreparationSectionID = iStatistics::getInstance().registerSection("scene", iaColor4f(1, 1, 0, 1), 3);
        _postRenderSectionID = iStatistics::getInstance().registerSection("postRender", iaColor4f(0, 1, 1, 1), 3);
    }

    iView::~iView()
    {
        if (_scenePreparationSectionID != 0)
        {
            iStatistics::getInstance().unregisterSection(_scenePreparationSectionID);
            _scenePreparationSectionID = 0;
        }

        if (_postRenderSectionID != 0)
        {
            iStatistics::getInstance().unregisterSection(_postRenderSectionID);
            _postRenderSectionID = 0;
        }

        if (_renderEvent.hasDelegates())
        {
            con_warn("not all delegates unregistered");
            _renderEvent.clear();
        }
    }

    void iView::setName(const iaString& name)
    {
        _name = name;

        if (_scenePreparationSectionID != 0)
        {
            iStatistics::getInstance().unregisterSection(_scenePreparationSectionID);
            _scenePreparationSectionID = 0;
        }

        if (_postRenderSectionID != 0)
        {
            iStatistics::getInstance().unregisterSection(_postRenderSectionID);
            _postRenderSectionID = 0;
        }

        iaString scene = _name;
        scene += ":scene";
        _scenePreparationSectionID = iStatistics::getInstance().registerSection(scene, iaColor4f(1, 0, 0, 1), 3);

        iaString post = _name;
        post += ":postRender";
        _postRenderSectionID = iStatistics::getInstance().registerSection(post, iaColor4f(0, 1, 0, 1), 3);
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
        _viewRect = rect;
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

    void iView::draw()
    {
        iRenderer::getInstance().setViewport(_resultingRectangle.getX(), _resultingRectangle.getY(), _resultingRectangle.getWidth(), _resultingRectangle.getHeight());

        iRenderer::getInstance().setClearColor(_clearColor);
        iRenderer::getInstance().setClearDepth(_clearDepth);

        if (_clearColorActive)
        {
            iRenderer::getInstance().clearColorBuffer();
        }

        if (_clearDepthActive)
        {
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

        iStatistics::getInstance().beginSection(_scenePreparationSectionID);
        if (_scene != nullptr)
        {
            _scene->handle();
        }
        iStatistics::getInstance().endSection(_scenePreparationSectionID);

        if (_scene != nullptr)
        {
            _renderEngine.render();
        }
        
        iStatistics::getInstance().beginSection(_postRenderSectionID);
        _renderEvent();
        iStatistics::getInstance().endSection(_postRenderSectionID);
    }

    void iView::updateWindowRect(const iRectanglei& windowRect)
    {
        _windowRect = windowRect;

        _resultingRectangle.setX(_viewRect.getX() * _windowRect.getWidth() + 0.5f);
        _resultingRectangle.setY(_viewRect.getY() * _windowRect.getHeight() + 0.5f);
        _resultingRectangle.setWidth(_viewRect.getWidth() * _windowRect.getWidth() + 0.5f);
        _resultingRectangle.setHeight(_viewRect.getHeight() * _windowRect.getHeight() + 0.5f);
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
        return static_cast<float32>(_resultingRectangle.getWidth()) / static_cast<float32>(_resultingRectangle.getHeight());
    }

    iaVector3f iView::unProject(const iaVector3f& screenpos, const iaMatrixf& modelMatrix)
    {
        iaMatrixf viewMatrix;
        viewMatrix.lookAt(modelMatrix._pos, modelMatrix._pos - modelMatrix._depth, modelMatrix._top);

        iaMatrixf modelViewMatrix = viewMatrix;
        modelViewMatrix *= modelMatrix;

        iaMatrixf projectionMatrix;
        projectionMatrix.perspective(_viewAngel, getAspectRatio(), _nearPlaneDistance, _farPlaneDistance);

        return iRenderer::getInstance().unProject(screenpos, modelViewMatrix, projectionMatrix, _resultingRectangle);
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
