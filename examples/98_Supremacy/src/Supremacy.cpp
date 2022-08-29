// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "Supremacy.h"

Supremacy::Supremacy(iWindow *window)
    : iLayer(window, L"Supremacy"), _viewOrtho(iView(false))
{
}

Supremacy::~Supremacy()
{
}

void Supremacy::onInit()
{
    _viewOrtho.setClearColor(0.55, 0.0, 0.0, 1.0);

    _viewOrtho.setName("view ortho");
    _viewOrtho.setClearColor(true);
    _viewOrtho.setClearDepth(true);
    _viewOrtho.setOrthogonal(0.0, static_cast<float32>(getWindow()->getClientWidth()), static_cast<float32>(getWindow()->getClientHeight()), 0.0);
    _viewOrtho.registerRenderDelegate(iDrawDelegate(this, &Supremacy::onRenderOrtho));
    getWindow()->addView(&_viewOrtho, getZIndex() + 1);
}

void Supremacy::onDeinit()
{    
    getWindow()->removeView(&_viewOrtho);
    _viewOrtho.unregisterRenderDelegate(iDrawDelegate(this, &Supremacy::onRenderOrtho));
}

void Supremacy::onPreDraw()
{
}

void Supremacy::onEvent(iEvent &event)
{
    event.dispatch<iEventKeyDown>(IGOR_BIND_EVENT_FUNCTION(Supremacy::onKeyDown));
    event.dispatch<iEventKeyUp>(IGOR_BIND_EVENT_FUNCTION(Supremacy::onKeyUp));
}

void Supremacy::onRenderOrtho()
{
    iaMatrixd matrix;
    iRenderer::getInstance().setViewMatrix(matrix);
    matrix.translate(0, 0, -1);
    iRenderer::getInstance().setModelMatrix(matrix);

    // TODO draw shit
}

bool Supremacy::onKeyDown(iEventKeyDown &event)
{
    switch (event.getKey())
    {
    case iKeyCode::W:
        return true;

    case iKeyCode::A:
        return true;

    case iKeyCode::S:
        return true;

    case iKeyCode::D:
        return true;

    case iKeyCode::I:
        return true;

    case iKeyCode::J:
        return true;

    case iKeyCode::K:
        return true;

    case iKeyCode::L:
        return true;        

    case iKeyCode::ESC:
        iApplication::getInstance().stop();
        return true;
    }

    return false;
}

bool Supremacy::onKeyUp(iEventKeyUp &event)
{
    switch (event.getKey())
    {
    case iKeyCode::W:
        return true;

    case iKeyCode::A:
        return true;

    case iKeyCode::S:
        return true;

    case iKeyCode::D:
        return true;

    case iKeyCode::I:
        return true;

    case iKeyCode::J:
        return true;

    case iKeyCode::K:
        return true;

    case iKeyCode::L:
        return true;        
    }

    return false;
}