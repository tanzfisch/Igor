// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/nodes/iNodeLight.h>

#include <iaux/system/iaConsole.h>
#include <igor/scene/iScene.h>

namespace igor
{

    iNodeLight::iNodeLight()
        : iNode()
    {
        setName(L"iNodeLight");
        _nodeType = iNodeType::iNodeLight;
        _nodeKind = iNodeKind::Light;
    }

    iNodeLight::iNodeLight(iNodeLight *node)
        : iNode()
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeType = node->_nodeType;
        _nodeKind = node->_nodeKind;

        setName(node->getName());
        setAmbient(node->getAmbient());
        setDiffuse(node->getDiffuse());
        setSpecular(node->getSpecular());
        setType(node->getType());
    }

    iNodeLight::~iNodeLight()
    {
        setScene(nullptr);
    }

    void iNodeLight::setType(iLightType type)
    {
        _type = type;
    }

    iLightType iNodeLight::getType() const
    {
        return _type;
    }

    const iaVector3d &iNodeLight::getPosition() const
    {
        return _worldMatrix._pos;
    }

    const iaVector3d &iNodeLight::getDirection() const
    {
        return _worldMatrix._pos;
    }

    void iNodeLight::getWorldMatrix(iaMatrixd &matrix)
    {
        matrix = _worldMatrix;
    }

    void iNodeLight::onUpdateTransform(iaMatrixd &matrix)
    {
        _worldMatrix = matrix;
    }

    iaColor3f &iNodeLight::getAmbient()
    {
        return _ambient;
    }

    void iNodeLight::setDiffuse(float32 r, float32 g, float32 b)
    {
        setDiffuse(iaColor3f(r, g, b));
    }

    void iNodeLight::setAmbient(float32 r, float32 g, float32 b)
    {
        setAmbient(iaColor3f(r, g, b));
    }

    void iNodeLight::setSpecular(float32 r, float32 g, float32 b)
    {
        setSpecular(iaColor3f(r, g, b));
    }

    void iNodeLight::setAmbient(const iaColor3f &color)
    {
        _ambient = color;
    }

    iaColor3f &iNodeLight::getDiffuse()
    {
        return _diffuse;
    }

    void iNodeLight::setDiffuse(const iaColor3f &color)
    {
        _diffuse = color;
    }

    iaColor3f &iNodeLight::getSpecular()
    {
        return _specular;
    }

    void iNodeLight::setSpecular(const iaColor3f &color)
    {
        _specular = color;
    }

    void iNodeLight::onPreSetScene()
    {
        if (getScene())
        {
            getScene()->unregisterLight(this);
        }
    }

    void iNodeLight::onPostSetScene()
    {
        if (getScene())
        {
            getScene()->registerLight(this);
        }
    }

}; // namespace igor
