// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
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
        if (iRenderer::getInstance().isReady())
        {
            deactivateShader();
            clearShader();

            _shader = new iShader();

            for (auto source : _shaderSources)
            {
                _shader->loadFile(source._filename, source._type);
            }

            _shader->compile();

            uint32 program = _shader->getProgram();

            _lightOrientation = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_LIGHT_ORIENTATION);
            _lightAmbient = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_LIGHT_AMBIENT);
            _lightDiffuse = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_LIGHT_DIFFUSE);
            _lightSpecular = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_LIGHT_SPECULAR);

            if (_lightOrientation != -1 &&
                _lightAmbient != -1 &&
                _lightDiffuse != -1 &&
                _lightSpecular != -1)
            {
                _hasDirectionalLight = true;
            }

            _eyePosition = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_EYE_POSITION);
            if (_eyePosition != -1)
            {
                _hasEyePosition = true;
            }

            _mvp_matrix = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_MODEL_VIEW_PROJECTION);
            if (_mvp_matrix != -1)
            {
                _hasModelViewProjectionMatrix = true;
            }

            _model_matrix = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_MODEL);
            if (_mvp_matrix != -1)
            {
                _hasModelMatrix = true;
            }

            _matAmbient = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_MATERIAL_AMBIENT);
            _matDiffuse = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_MATERIAL_DIFFUSE);
            _matSpecular = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_MATERIAL_SPECULAR);
            _matShininess = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_MATERIAL_SHININESS);
            _matEmissive = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_MATERIAL_EMISSIVE);

            if (_matAmbient != -1 &&
                _matDiffuse != -1 &&
                _matSpecular != -1 &&
                _matShininess != -1 &&
                _matEmissive != -1)
            {
                _hasTargetMaterial = true;
            }

            _matSolidColor = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_SOLIDCOLOR);
            if (_matSolidColor != -1)
            {
                _hasSolidColor = true;
            }

            _hasTexture[0] = iRenderer::getInstance().getShaderPropertyID(program, SAMPLER_TEXTURE0) != -1 ? true : false;
            _hasTexture[1] = iRenderer::getInstance().getShaderPropertyID(program, SAMPLER_TEXTURE1) != -1 ? true : false;
            _hasTexture[2] = iRenderer::getInstance().getShaderPropertyID(program, SAMPLER_TEXTURE2) != -1 ? true : false;
            _hasTexture[3] = iRenderer::getInstance().getShaderPropertyID(program, SAMPLER_TEXTURE3) != -1 ? true : false;
        }
    }

    void iMaterial::addShaderSource(iaString filename, iShaderObjectType type)
    {
        for (auto source : _shaderSources)
        {
            if (source._filename == filename)
            {
                con_warn("shader " << filename << " already added");
                return;
            }
        }

        iShaderSource shaderSource;
        shaderSource._filename = filename;
        shaderSource._type = type;
        _shaderSources.push_back(shaderSource);
    }

    vector<iShaderSource> iMaterial::getShaderSources() const
    {
        return _shaderSources;
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

