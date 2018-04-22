// Igor game engine
// (c) Copyright 2012-2018 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeLight.h>

#include <iaConsole.h>
#include <iScene.h>

namespace Igor
{
	
	iNodeLight::iNodeLight()
		: iNode()
	{
		setName(L"iNodeLight");
        _nodeType = iNodeType::iNodeLight;
		_nodeKind = iNodeKind::Light;
	}

    iNodeLight::iNodeLight(iNodeLight* node)
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

    iLightType iNodeLight::getType()
    {
        return _type;
    }

    iaVector4d iNodeLight::getPosition()
    {
        iaVector4d result;
        result._vec = _worldMatrix._pos;
        result._w = 1.0f;
        return result;
    }

    iaVector4d iNodeLight::getDirection()
    {
        iaVector4d result;
        result._vec = _worldMatrix._pos;
        result._w = 0.0f;

        return result;
    }
    
	void iNodeLight::getWorldMatrix(iaMatrixd& matrix)
	{
		matrix = _worldMatrix;
	}

    void iNodeLight::onUpdateTransform(iaMatrixd& matrix)
	{
		_worldMatrix = matrix;
	}

    iaColor4f& iNodeLight::getAmbient()
    {
        return _ambient;
    }

    void iNodeLight::setAmbient(iaColor4f& color)
    {
        _ambient = color;
    }

    iaColor4f& iNodeLight::getDiffuse()
    {
        return _diffuse;
    }

    void iNodeLight::setDiffuse(iaColor4f& color)
    {
        _diffuse = color;
    }

    iaColor4f& iNodeLight::getSpecular()
    {
        return _specular;
    }

    void iNodeLight::setSpecular(iaColor4f& color)
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

};
