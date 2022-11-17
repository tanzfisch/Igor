// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/material/iMaterial.h>

#include <igor/renderer/iRenderer.h>
#include <igor/scene/nodes/iNodeLight.h>
#include <igor/scene/nodes/iNodeCamera.h>
#include <igor/resources/material/iShader_Old.h>

#include <sstream>

namespace igor
{

    iaIDGenerator64 iMaterial_old::_idGenerator;

    iMaterial_old::iMaterial_old()
    {
        _id = _idGenerator.createID();
    }

    iMaterial_old::~iMaterial_old()
    {
        clearShader();
    }

    void iMaterial_old::clearShader()
    {
        if (_shader != nullptr)
        {
            delete _shader;
            _shader = nullptr;
        }
    }

    iMaterialID_old iMaterial_old::getID() const
    {
        return _id;
    }

    void iMaterial_old::compileShader()
    {
        if (iRenderer::getInstance().isReady())
        {
            deactivateShader();
            clearShader();

            _shader = new iShader_Old();

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
            if (_model_matrix != -1)
            {
                _hasModelMatrix = true;
            }

            _matAmbient = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_MATERIAL_AMBIENT);
            _matDiffuse = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_MATERIAL_DIFFUSE);
            _matSpecular = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_MATERIAL_SPECULAR);
            _matShininess = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_MATERIAL_SHININESS);
            _matAlpha = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_MATERIAL_ALPHA);
            _matEmissive = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_MATERIAL_EMISSIVE);

            if (_matAmbient != -1 &&
                _matDiffuse != -1 &&
                _matSpecular != -1 &&
                _matShininess != -1 &&
                _matEmissive != -1 &&
                _matAlpha != -1)
            {
                _hasTargetMaterial = true;
            }

            _matSolidColor = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_SOLIDCOLOR);
            if (_matSolidColor != -1)
            {
                _hasSolidColor = true;
            }

            for (int i = 0; i < MAX_TEXTURE_UNITS; ++i)
            {
                std::stringstream shaderProperty;
                shaderProperty << SAMPLER_TEXTURE << i;
                _matTexture[i] = iRenderer::getInstance().getShaderPropertyID(program, shaderProperty.str().c_str());
                _hasTexture[i] = _matTexture[i] != -1 ? true : false;
            }
        }
    }

    bool iMaterial_old::isValid() const
    {
        return _isValid;
    }

    void iMaterial_old::setRenderState(const iRenderState state, const iRenderStateValue value)
    {
        _renderStateSet.setRenderState(state, value);
    }

    void iMaterial_old::addShaderSource(iaString filename, iShaderObjectType type)
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

    std::vector<iShaderSource> iMaterial_old::getShaderSources() const
    {
        return _shaderSources;
    }

    iShader_Old *iMaterial_old::getShader()
    {
        return _shader;
    }

    int32 iMaterial_old::getOrder() const
    {
        return _order;
    }

    void iMaterial_old::setOrder(int32 order)
    {
        con_assert(order >= RENDER_ORDER_MIN && order <= RENDER_ORDER_MAX, "out of bounds");

        _order = order;
    }

    iRenderStateSet &iMaterial_old::getRenderStateSet()
    {
        return _renderStateSet;
    }

    void iMaterial_old::activateShader()
    {
        if (nullptr != _shader)
        {
            _shader->enable();
        }
    }

    void iMaterial_old::deactivateShader()
    {
        if (nullptr != _shader)
        {
            _shader->disable();
        }
    }

    iaString iMaterial_old::getName()
    {
        return _name;
    }

    void iMaterial_old::setName(iaString name)
    {
        _name = name;
    }

} // namespace igor
