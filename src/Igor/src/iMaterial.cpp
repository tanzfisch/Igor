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

            int32 lightOrientation = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_LIGHT_ORIENTATION);
            int32 lightAmbient = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_LIGHT_AMBIENT);
            int32 lightDiffuse = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_LIGHT_DIFFUSE);
            int32 lightSpecular = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_LIGHT_SPECULAR);

            if (lightOrientation != -1 &&
                lightAmbient != -1 &&
                lightDiffuse != -1 &&
                lightSpecular != -1)
            {
                _hasDirectionalLight = true;
            }

            int32 eyePosition = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_EYE_POSITION);
            if (eyePosition != -1)
            {
                _hasEyePosition = true;
            }

            int32 mvp_matrix = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_MODEL_VIEW_PROJECTION);
            if (mvp_matrix != -1)
            {
                _hasModelViewProjectionMatrix = true;
            }

            int32 model_matrix = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_MODEL);
            if (mvp_matrix != -1)
            {
                _hasModelMatrix = true;
            }

            int32 matAmbient = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_MATERIAL_AMBIENT);
            int32 matDiffuse = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_MATERIAL_DIFFUSE);
            int32 matSpecular = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_MATERIAL_SPECULAR);
            int32 matShininess = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_MATERIAL_SHININESS);
            int32 matEmissive = iRenderer::getInstance().getShaderPropertyID(program, UNIFORM_MATERIAL_EMISSIVE);

            if (matAmbient != -1 &&
                matDiffuse != -1 &&
                matSpecular != -1 &&
                matShininess != -1 &&
                matEmissive != -1)
            {
                _hasTargetMaterial = true;
            }
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

