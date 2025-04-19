// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iLightComponent.h>

namespace igor
{
    iLightComponent::iLightComponent()
    {
    }

    iEntityComponent *iLightComponent::createInstance()
    {
        return new iLightComponent();
    }

    const iaString& iLightComponent::getTypeName()
    {
        static const iaString name("igor_light_component");
        return name;
    }     

    iEntityComponentPtr iLightComponent::getCopy()
    {
        iLightComponent *component = new iLightComponent();
        component->_ambient = _ambient;
        component->_diffuse = _diffuse;
        component->_specular = _specular;
        component->_type = _type;
        return component;
    }

    void iLightComponent::setType(iLightType type)
    {
        _type = type;
    }

    iLightType iLightComponent::getType() const
    {
        return _type;
    }

    iaColor3f &iLightComponent::getAmbient()
    {
        return _ambient;
    }

    void iLightComponent::setDiffuse(float32 r, float32 g, float32 b)
    {
        setDiffuse(iaColor3f(r, g, b));
    }

    void iLightComponent::setAmbient(float32 r, float32 g, float32 b)
    {
        setAmbient(iaColor3f(r, g, b));
    }

    void iLightComponent::setSpecular(float32 r, float32 g, float32 b)
    {
        setSpecular(iaColor3f(r, g, b));
    }

    void iLightComponent::setAmbient(const iaColor3f &color)
    {
        _ambient = color;
    }

    iaColor3f &iLightComponent::getDiffuse()
    {
        return _diffuse;
    }

    void iLightComponent::setDiffuse(const iaColor3f &color)
    {
        _diffuse = color;
    }

    iaColor3f &iLightComponent::getSpecular()
    {
        return _specular;
    }

    void iLightComponent::setSpecular(const iaColor3f &color)
    {
        _specular = color;
    }
}