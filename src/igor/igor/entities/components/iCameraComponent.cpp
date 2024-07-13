// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iCameraComponent.h>

#include <igor/renderer/iRenderer.h>

namespace igor
{
    iCameraComponent::iCameraComponent(const iaString &name)
        : iEntityComponent(name)
    {
    }

    void iCameraComponent::setViewportRelative(const iaRectangled &rect)
    {
        _viewport = rect;
    }

    const iaRectangled &iCameraComponent::getViewport() const
    {
        return _viewport;
    }

    void iCameraComponent::setPerspective(float64 viewAngel)
    {
        con_assert(viewAngel > 0.0 && viewAngel < 180.0, "value out of range");

        _fieldOfView = viewAngel;
        _projection = iProjectionType::Perspective;
    }

    float64 iCameraComponent::getFieldOfView() const
    {
        return _fieldOfView;
    }

    void iCameraComponent::setOrthogonal(float32 left, float32 right, float32 bottom, float32 top)
    {
        _leftOrtho = left;
        _rightOrtho = right;
        _topOrtho = top;
        _bottomOrtho = bottom;
        _projection = iProjectionType::Orthogonal;
    }

    void iCameraComponent::setClipPlanes(float32 nearPlain, float32 farPlain)
    {
        _clipNear = nearPlain;
        _clipFar = farPlain;
    }

    float64 iCameraComponent::getNearClipPlane() const
    {
        return _clipNear;
    }
    float64 iCameraComponent::getFarClipPlane() const
    {
        return _clipFar;
    }

    void iCameraComponent::setClearColorActive(bool active)
    {
        _clearColorActive = active;
    }

    bool iCameraComponent::isClearColorActive() const
    {
        return _clearColorActive;
    }

    void iCameraComponent::setClearColor(const iaColor4f &color)
    {
        _clearColor = color;
    }

    void iCameraComponent::setClearColor(float32 r, float32 g, float32 b, float32 a)
    {
        _clearColor.set(r, g, b, a);
    }

    const iaColor4f &iCameraComponent::getClearColor() const
    {
        return _clearColor;
    }

    void iCameraComponent::setClearDepthActive(bool active)
    {
        _clearDepthActive = active;
    }

    bool iCameraComponent::isClearDepthActive() const
    {
        return _clearDepthActive;
    }

    void iCameraComponent::setClearDepth(float32 depth)
    {
        _clearDepth = depth;
    }

    float32 iCameraComponent::getClearDepth() const
    {
        return _clearDepth;
    }

    iProjectionType iCameraComponent::getProjectionType() const
    {
        return _projection;
    }

    float64 iCameraComponent::getLeftOrtho() const
    {
        return _leftOrtho;
    }

    float64 iCameraComponent::getRightOrtho() const
    {
        return _rightOrtho;
    }

    float64 iCameraComponent::getTopOrtho() const
    {
        return _topOrtho;
    }

    float64 iCameraComponent::getBottomOrtho() const
    {
        return _bottomOrtho;
    }

}