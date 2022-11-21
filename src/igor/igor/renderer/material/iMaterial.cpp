// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/material/iMaterial.h>

#include <igor/renderer/material/iMaterialReader.h>
#include <igor/renderer/utils/iRendererUtils.h>
#include <igor/resources/iResourceManager.h>

namespace igor
{

    void deleter(const iMaterial *material)
    {
        delete material;
    }

    iMaterialPtr iMaterial::create()
    {
        iMaterialPtr result = std::shared_ptr<iMaterial>(new iMaterial(), deleter);
        result->_uuid = iaUUID::create();
        return result;
    }

    iMaterialPtr iMaterial::create(const iaString &filename)
    {
        std::shared_ptr<iMaterial> result(new iMaterial(), deleter);
        iMaterialReader::read(iResourceManager::getInstance().getPath(filename), result);
        if (!result->_uuid.isValid())
        {
            result->_uuid = iaUUID::create();
        }
        return result;
    }

    void iMaterial::setShaderProgram(const iShaderProgramPtr &shaderProgram)
    {
        _shaderProgram = shaderProgram;
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

        (_renderStateSet.getRenderState(iRenderState::DepthTest) == iRenderStateValue::On) ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
        (_renderStateSet.getRenderState(iRenderState::DepthMask) == iRenderStateValue::On) ? glDepthMask(GL_TRUE) : glDepthMask(GL_FALSE);
        (_renderStateSet.getRenderState(iRenderState::Blend) == iRenderStateValue::On) ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
        (_renderStateSet.getRenderState(iRenderState::CullFace) == iRenderStateValue::On) ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);

        // TODO is this necessary???
        glActiveTexture(GL_TEXTURE0);
        (_renderStateSet.getRenderState(iRenderState::Texture2D0) == iRenderStateValue::On) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);

        glActiveTexture(GL_TEXTURE1);
        (_renderStateSet.getRenderState(iRenderState::Texture2D1) == iRenderStateValue::On) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);

        glActiveTexture(GL_TEXTURE2);
        (_renderStateSet.getRenderState(iRenderState::Texture2D2) == iRenderStateValue::On) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);

        glActiveTexture(GL_TEXTURE3);
        (_renderStateSet.getRenderState(iRenderState::Texture2D3) == iRenderStateValue::On) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);

        glActiveTexture(GL_TEXTURE4);
        (_renderStateSet.getRenderState(iRenderState::Texture2D4) == iRenderStateValue::On) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);

        glActiveTexture(GL_TEXTURE5);
        (_renderStateSet.getRenderState(iRenderState::Texture2D5) == iRenderStateValue::On) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);

        glActiveTexture(GL_TEXTURE6);
        (_renderStateSet.getRenderState(iRenderState::Texture2D6) == iRenderStateValue::On) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);

        glActiveTexture(GL_TEXTURE7);
        (_renderStateSet.getRenderState(iRenderState::Texture2D7) == iRenderStateValue::On) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);

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

        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); we assume this is a given at all times

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

    void iMaterial::setID(const iaUUID &uuid)
    {
        _uuid = uuid;
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

    void iMaterial::setMatrix(const iaString &uniform, const iaMatrixf &value)
    {
        _shaderProgram->setMatrix(uniform, value);
    }

    const iaUUID &iMaterial::getID() const
    {
        return _uuid;
    }

}
