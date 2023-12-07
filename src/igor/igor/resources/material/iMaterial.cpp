// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/material/iMaterial.h>

#include <igor/resources/material/iMaterialIO.h>
#include <igor/resources/iResourceManager.h>

#include <igor/renderer/utils/iRendererUtils.h>

#include <sstream>

namespace igor
{

    iMaterial::iMaterial(const iParameters &parameters)
    : iResource(parameters)
    {
        setName("Material Name");
    }

    bool iMaterial::isValid() const
    {
        if (_shaderProgram == nullptr)
        {
            return false;
        }

        return true;
    }

    void iMaterial::setShaderProgram(const iShaderProgramPtr &shaderProgram)
    {
        if (!shaderProgram->isValid())
        {
            return;
        }

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

        if (_shaderProgram->hasUniformLocation(UNIFORM_MODEL_VIEW))
        {
            _hasModelViewMatrix = true;
        }

        if (_shaderProgram->hasUniformLocation(UNIFORM_VIEW_PROJECTION))
        {
            _hasViewProjectionMatrix = true;
        }

        if (_shaderProgram->hasUniformLocation(UNIFORM_CONFIG_TILING))
        {
            _hasConfigTiling = true;
        }

        if (_shaderProgram->hasUniformLocation(UNIFORM_CONFIG_VELOCITY_ORIENTED))
        {
            _hasConfigVelocityOriented = true;
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

    bool iMaterial::hasModelViewMatrix() const
    {
        return _hasModelViewMatrix;
    }

    bool iMaterial::hasViewProjectionMatrix() const
    {
        return _hasViewProjectionMatrix;
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

    bool iMaterial::hasTilingConfig() const
    {
        return _hasConfigTiling;
    }

    bool iMaterial::hasVelocityOrientedConfig() const
    {
        return _hasConfigVelocityOriented;
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
        con_assert(isValid(), "invalid material \"" << getName() << "\" [" << getID() << "]");

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

        if (_renderStateSet.getRenderState(iRenderState::Wireframe) == iRenderStateValue::On)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
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

    int32 iMaterial::getOrder() const
    {
        return _order;
    }

    void iMaterial::setOrder(int32 order)
    {
        con_assert(order >= RENDER_ORDER_MIN && order <= RENDER_ORDER_MAX, "out of bounds");

        _order = order;
    }

    iMaterialVisibility iMaterial::getVisibility() const
    {
        return _visibility;
    }

    void iMaterial::setVisibility(iMaterialVisibility visibility)
    {
        _visibility = visibility;
    }

    std::wostream &operator<<(std::wostream &stream, const iMaterialVisibility &visibility)
    {
        const static std::wstring text[] = {
            L"Private",
            L"Public"};

        stream << text[static_cast<int>(visibility)];

        return stream;
    }

    const iaString &iMaterial::getFilename() const
    {
        return _filename;
    }
}
