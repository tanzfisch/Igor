// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/material/iMaterial.h>

#include <igor/resources/material/iMaterialReader.h>
#include <igor/resources/iResourceManager.h>

#include <igor/renderer/utils/iRendererUtils.h>

namespace igor
{

    void deleter(const iMaterial *material)
    {
        delete material;
    }

    iMaterialPtr iMaterial::create()
    {
        iMaterialPtr result = std::shared_ptr<iMaterial>(new iMaterial(), deleter);
        result->_materialID = iMaterialID::create();
        return result;
    }

    iMaterialPtr iMaterial::create(const iaString &filename)
    {
        std::shared_ptr<iMaterial> result(new iMaterial(), deleter);
        iMaterialReader::read(iResourceManager::getInstance().getPath(filename), result);
        if (!result->_materialID.isValid())
        {
            result->_materialID = iMaterialID::create();
        }
        return result;
    }

    void iMaterial::setShaderProgram(const iShaderProgramPtr &shaderProgram)
    {
        _shaderProgram = shaderProgram;

        if (_shaderProgram == nullptr)
        {
            _hasDirectionalLight = false;
            _hasEyePosition = false;
            _hasModelViewProjectionMatrix = false;
            _hasModelMatrix = false;
            _hasTargetMaterial = false;
            _hasSolidColor = false;
            return;
        }

        if (_shaderProgram->hasUniformLocation(UNIFORM_LIGHT_ORIENTATION) &&
            _shaderProgram->hasUniformLocation(UNIFORM_LIGHT_AMBIENT) &&
            _shaderProgram->hasUniformLocation(UNIFORM_LIGHT_DIFFUSE) &&
            _shaderProgram->hasUniformLocation(UNIFORM_LIGHT_SPECULAR))
        {
            _hasDirectionalLight = true;
        }

        if (_shaderProgram->hasUniformLocation(UNIFORM_EYE_POSITION))
        {
            _hasEyePosition = true;
        }

        if (_shaderProgram->hasUniformLocation(UNIFORM_MODEL_VIEW_PROJECTION))
        {
            _hasModelViewProjectionMatrix = true;
        }

        if (_shaderProgram->hasUniformLocation(UNIFORM_MODEL))
        {
            _hasModelMatrix = true;
        }

        if (_shaderProgram->hasUniformLocation(UNIFORM_MATERIAL_AMBIENT) &&
            _shaderProgram->hasUniformLocation(UNIFORM_MATERIAL_DIFFUSE) &&
            _shaderProgram->hasUniformLocation(UNIFORM_MATERIAL_SPECULAR) &&
            _shaderProgram->hasUniformLocation(UNIFORM_MATERIAL_SHININESS) &&
            _shaderProgram->hasUniformLocation(UNIFORM_MATERIAL_ALPHA) &&
            _shaderProgram->hasUniformLocation(UNIFORM_MATERIAL_EMISSIVE))
        {
            _hasTargetMaterial = true;
        }

        if (_shaderProgram->hasUniformLocation(UNIFORM_SOLIDCOLOR))
        {
            _hasSolidColor = true;
        }

        for (int i = 0; i < MAX_TEXTURE_UNITS; ++i)
        {
            std::stringstream shaderProperty;
            shaderProperty << SAMPLER_TEXTURE << i;
            _hasTexture[i] = _shaderProgram->hasUniformLocation(shaderProperty.str().c_str());
        }        
    }

    bool iMaterial::hasDirectionalLight() const
    {
        return _hasDirectionalLight;
    }

    bool iMaterial::hasEyePosition() const
    {
        return _hasEyePosition;
    }

    bool iMaterial::hasModelViewProjectionMatrix() const
    {
        return _hasModelViewProjectionMatrix;
    }

    bool iMaterial::hasModelMatrix() const
    {
        return _hasModelMatrix;
    }

    bool iMaterial::hasTargetMaterial() const
    {
        return _hasTargetMaterial;
    }

    bool iMaterial::hasSolidColor() const
    {
        return _hasSolidColor;
    }

    bool iMaterial::hasTextureUnit(uint32 texUnit) const
    {
        con_assert(texUnit < MAX_TEXTURE_UNITS, "out of bounds");
        return _hasTexture[texUnit];
    }    

    iShaderProgramPtr iMaterial::getShaderProgram() const
    {
        return _shaderProgram;
    }

    void iMaterial::unbind()
    {
        _shaderProgram->unbind();
    }

    void iMaterial::bind()
    {
        _shaderProgram->bind();

        if (_renderStateSet.getRenderState(iRenderState::DepthTest) == iRenderStateValue::On)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }

        if (_renderStateSet.getRenderState(iRenderState::DepthMask) == iRenderStateValue::On)
        {
            glDepthMask(GL_TRUE);
        }
        else
        {
            glDepthMask(GL_FALSE);
        }

        if (_renderStateSet.getRenderState(iRenderState::Blend) == iRenderStateValue::On)
        {
            glEnable(GL_BLEND);
        }
        else
        {
            glDisable(GL_BLEND);
        }

        if (_renderStateSet.getRenderState(iRenderState::CullFace) == iRenderStateValue::On)
        {
            glEnable(GL_CULL_FACE);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }        

        switch (_renderStateSet.getRenderState(iRenderState::DepthFunc))
        {
        case iRenderStateValue::Less:
            glDepthFunc(GL_LESS);
            break;

        case iRenderStateValue::LessOrEqual:
            glDepthFunc(GL_LEQUAL);
            break;

        case iRenderStateValue::Never:
            glDepthFunc(GL_NEVER);
            break;

        case iRenderStateValue::Equal:
            glDepthFunc(GL_EQUAL);
            break;

        case iRenderStateValue::Greater:
            glDepthFunc(GL_GREATER);
            break;

        case iRenderStateValue::NotEqual:
            glDepthFunc(GL_NOTEQUAL);
            break;

        case iRenderStateValue::GreaterOrEqual:
            glDepthFunc(GL_GEQUAL);
            break;

        case iRenderStateValue::Always:
            glDepthFunc(GL_ALWAYS);
            break;
        }

        switch (_renderStateSet.getRenderState(iRenderState::CullFaceFunc))
        {
        case iRenderStateValue::Front:
            glCullFace(GL_FRONT);
            break;

        case iRenderStateValue::Back:
            glCullFace(GL_BACK);
            break;
        }

        if (_renderStateSet.getRenderState(iRenderState::Wireframe) == iRenderStateValue::On) // TODO || forceWireframe
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }

    void iMaterial::setName(const iaString &name)
    {
        _name = name;
    }

    void iMaterial::setID(const iMaterialID &materialID)
    {
        _materialID = materialID;
    }

    const iaString &iMaterial::getName() const
    {
        return _name;
    }

    void iMaterial::setRenderState(const iRenderState state, const iRenderStateValue value)
    {
        _renderStateSet.setRenderState(state, value);
    }

    iRenderStateValue iMaterial::getRenderState(const iRenderState state) const
    {
        return _renderStateSet.getRenderState(state);
    }

    void iMaterial::setInt(const iaString &uniform, int value)
    {
        _shaderProgram->setInt(uniform, value);
    }

    void iMaterial::setFloat(const iaString &uniform, float32 value)
    {
        _shaderProgram->setFloat(uniform, value);
    }

    void iMaterial::setFloat2(const iaString &uniform, const iaVector2f &value)
    {
        _shaderProgram->setFloat2(uniform, value);
    }

    void iMaterial::setFloat3(const iaString &uniform, const iaVector3f &value)
    {
        _shaderProgram->setFloat3(uniform, value);
    }

    void iMaterial::setFloat4(const iaString &uniform, const iaVector4f &value)
    {
        _shaderProgram->setFloat4(uniform, value);
    }

    void iMaterial::setFloat3(const iaString &uniform, const iaColor3f &value)
    {
        _shaderProgram->setFloat3(uniform, iaVector3f(value._r, value._g, value._b));
    }

    void iMaterial::setFloat4(const iaString &uniform, const iaColor4f &value)
    {
        _shaderProgram->setFloat4(uniform, iaVector4f(value._r, value._g, value._b, value._a));
    }

    void iMaterial::setMatrix(const iaString &uniform, const iaMatrixf &value)
    {
        _shaderProgram->setMatrix(uniform, value);
    }

    const iMaterialID &iMaterial::getID() const
    {
        return _materialID;
    }

    int32 iMaterial::getOrder() const
    {
        return _order;
    }

    void iMaterial::setOrder(int32 order)
    {
        con_assert(order >= RENDER_ORDER_MIN && order <= RENDER_ORDER_MAX, "out of bounds");

        _order = order;
    }

}
