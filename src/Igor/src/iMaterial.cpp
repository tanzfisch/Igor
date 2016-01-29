// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iMaterial.h>

#include <iRenderer.h>
#include <iNodeLight.h>
#include <iNodeCamera.h>
#include <iShader.h>

namespace Igor
{

    iMaterial::iMaterial()
    {

    }

	iMaterial::~iMaterial()
	{
		if(_shader != nullptr)
		{
			delete _shader;
		}
	}

    void iMaterial::compileShader()
    {
        if (_shader != nullptr)
        {
            _shader->compile();
        }
    }

    void iMaterial::addShaderSource(iaString filename, iShaderObjectType type)
    {
        if (_shader == nullptr)
        {
            _shader = new iShader();
        }

        _shader->loadFile(filename, type);
    }

    const vector<iShaderSource>& iMaterial::getShaderSources() const
    {
        if (_shader != nullptr)
        {
            return _shader->getShaderSources();
        }
    }

	iShader* iMaterial::getShader()
	{
		return _shader;
	}

    int32 iMaterial::getOrder()
    {
        return _order;
    }

    void iMaterial::setOrder(int32 order)
    {
        _order = order;
    }

	iRenderStateSet& iMaterial::getRenderStateSet()
	{
		return _renderStateSet;
	}

    void iMaterial::activateShader()
	{
        if (nullptr != _shader)
        {
            _shader->enable();
        }
	}

	void iMaterial::deactivateShader()
	{
        if (nullptr != _shader)
        {
            _shader->disable();
        }
	}

	iaString iMaterial::getName()
	{
		return _name;
	}

	void iMaterial::setName(iaString name)
	{
		_name = name;
	}

}

