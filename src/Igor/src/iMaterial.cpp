// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
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
        clearShader();
	}

    void iMaterial::clearShader()
    {
        if (_shader != nullptr)
        {
            delete _shader;
            _shader = nullptr;
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
        con_assert(_shader != nullptr, "zero pointer");

        // if you end up here with a zero pointer use hasShader() to check first
        return _shader->getShaderSources();
    }

    bool iMaterial::hasShader()
    {
        return _shader != nullptr ? true : false;
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
        con_assert(order >= RENDER_ORDER_MIN && order <= RENDER_ORDER_MAX, "out of bounds");

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

