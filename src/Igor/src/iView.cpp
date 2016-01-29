// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iView.h>

#include <iWindow.h>
#include <iRenderer.h>
#include <iTextureResourceFactory.h>
#include <iScene.h>
#include <iOctree.h>

#include <iaConsole.h>
using namespace IgorAux;

#include <algorithm>
using namespace std;

namespace Igor
{

    iView::~iView()
    {
        if (_renderEvent.hasDelegates())
        {
            con_err("not all delegates unregistered");
        }
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

    void iView::draw(const iRectanglei& rect, float32 aspectRatio)
    {
        iRenderer::getInstance().setViewport(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight());

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
            iRenderer::getInstance().setPerspective(_viewAngel, aspectRatio, _nearPlaneDistance, _farPlaneDistance);
        }
        else
        {
            iRenderer::getInstance().setOrtho(_left, _right, _bottom, _top, _nearPlaneDistance, _farPlaneDistance);
        }

        if (_scene != nullptr)
        {
            _scene->handle();
            _renderEngine.render();
        }
        
        _renderEvent();
    }

    void iView::draw(iWindow* window)
    {
        float32 windowClientWidth = window->getClientWidth();
        float32 windowClientHeight = window->getClientHeight();

        iRectanglei resultingRectangle(_viewRect.getX() * windowClientWidth + 0.5f,
            _viewRect.getY() * windowClientHeight + 0.5f,
            _viewRect.getWidth() * windowClientWidth + 0.5f,
            _viewRect.getHeight() * windowClientHeight + 0.5f);

        draw(resultingRectangle, (_viewRect.getWidth() * windowClientWidth) / (_viewRect.getHeight() * windowClientHeight));
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
