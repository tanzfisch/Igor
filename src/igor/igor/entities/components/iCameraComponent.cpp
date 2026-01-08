// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iCameraComponent.h>

#include <igor/renderer/iRenderer.h>

namespace igor
{
    iCameraComponent::iCameraComponent()
    {
    }

    iEntityComponentPtr iCameraComponent::createInstance()
    {
        return new iCameraComponent();
    }

    const iaString &iCameraComponent::getTypeName()
    {
        static const iaString name("igor_camera_component");
        return name;
    }

    iEntityComponentPtr iCameraComponent::getCopy()
    {
        iCameraComponent *component = new iCameraComponent();
        component->_viewport = _viewport;
        component->_projection = _projection;
        component->_fieldOfView = _fieldOfView;
        component->_clipNear = _clipNear;
        component->_clipFar = _clipFar;
        component->_clearColorActive = _clearColorActive;
        component->_clearDepthActive = _clearDepthActive;
        component->_clearColor = _clearColor;
        component->_clearDepth = _clearDepth;
        component->_leftOrtho = _leftOrtho;
        component->_rightOrtho = _rightOrtho;
        component->_topOrtho = _topOrtho;
        component->_bottomOrtho = _bottomOrtho;
        component->_offset = _offset;
        return component;
    }

    void iCameraComponent::setViewportRelative(const iaRectangled &rect)
    {
        _viewport = rect;
        increaseVersion();
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
        increaseVersion();
    }

    float64 iCameraComponent::getFieldOfView() const
    {
        return _fieldOfView;
    }

    void iCameraComponent::setOrthogonal(float32 left, float32 right, float32 bottom, float32 top)
    {
        _leftOrtho = left;
        _rightOrtho = right;
        _bottomOrtho = bottom;
        _topOrtho = top;
        _projection = iProjectionType::Orthogonal;
        increaseVersion();
    }

    void iCameraComponent::setClipPlanes(float32 nearPlain, float32 farPlain)
    {
        _clipNear = nearPlain;
        _clipFar = farPlain;
        increaseVersion();
    }

    float64 iCameraComponent::getNearPlane() const
    {
        return _clipNear;
    }
    float64 iCameraComponent::getFarPlane() const
    {
        return _clipFar;
    }

    void iCameraComponent::setClearColorActive(bool active)
    {
        _clearColorActive = active;
        increaseVersion();
    }

    bool iCameraComponent::isClearColorActive() const
    {
        return _clearColorActive;
    }

    void iCameraComponent::setClearColor(const iaColor4f &color)
    {
        _clearColor = color;
        increaseVersion();
    }

    void iCameraComponent::setClearColor(float32 r, float32 g, float32 b, float32 a)
    {
        _clearColor.set(r, g, b, a);
        increaseVersion();
    }

    const iaColor4f &iCameraComponent::getClearColor() const
    {
        return _clearColor;
    }

    void iCameraComponent::setClearDepthActive(bool active)
    {
        _clearDepthActive = active;
        increaseVersion();
    }

    bool iCameraComponent::isClearDepthActive() const
    {
        return _clearDepthActive;
    }

    void iCameraComponent::setClearDepth(float32 depth)
    {
        _clearDepth = depth;
        increaseVersion();
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

    std::vector<iaString> iCameraComponent::getInfo() const
    {
        std::vector<iaString> result = iEntityComponent::getInfo();

        result.push_back(iaString("Viewport: ") + iaString::toString(_viewport._x) + ", " + iaString::toString(_viewport._y) + ", " + iaString::toString(_viewport._width) + ", " + iaString::toString(_viewport._height));
        result.push_back(iaString("Projection: ") + (_projection == iProjectionType::Perspective ? "Perspective" : "Orthogonal"));
        result.push_back(iaString("FOV: ") + iaString::toString(_fieldOfView));
        result.push_back(iaString("Clip planes: ") + iaString::toString(_clipNear) + " - " + iaString::toString(_clipFar));
        result.push_back(iaString("Clear color: ") + iaString::toString(_clearColor._r) + ", " + iaString::toString(_clearColor._g) + ", " + iaString::toString(_clearColor._b) + ", " + iaString::toString(_clearColor._a) + " [" + (_clearColorActive ? "On" : "Off") + "]");
        result.push_back(iaString("Clear depth: ") + iaString::toString(_clearDepth) + " [" + (_clearDepthActive ? "On" : "Off") + "]");
        result.push_back(iaString("Ortho: ") + iaString::toString(_leftOrtho) + ", " + iaString::toString(_rightOrtho) + ", " + iaString::toString(_bottomOrtho) + ", " + iaString::toString(_topOrtho));
        result.push_back(iaString("Offset Pos: ") + iaString::toString(_offset._position._x) + ", " + iaString::toString(_offset._position._y) + ", " + iaString::toString(_offset._position._z));
        const auto euler = _offset._orientation.toEuler();
        result.push_back(iaString("Offset Ori: ") + iaString::toString(euler._x) + ", " + iaString::toString(euler._y) + ", " + iaString::toString(euler._z));
        result.push_back(iaString("Offset Scale: ") + iaString::toString(_offset._scale._x) + ", " + iaString::toString(_offset._scale._y) + ", " + iaString::toString(_offset._scale._z));

        return result;
    }

    void iCameraComponent::setOffset(const iaTransformd &offset)
    {
        _offset = offset;
    }

    const iaTransformd &iCameraComponent::getOffset() const
    {
        return _offset;
    }

}