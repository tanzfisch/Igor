// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/material/iMaterial.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{
    iMaterial::iMaterial(const iParameters &parameters)
        : iResource(parameters)
    {
        _emissive = parameters.getParameter<iaColor3f>(IGOR_RESOURCE_PARAM_EMISSIVE, iaColor3f(0, 0, 0));
        _ambient = parameters.getParameter<iaColor3f>(IGOR_RESOURCE_PARAM_AMBIENT, iaColor3f(0.4f, 0.4f, 0.4f));
        _diffuse = parameters.getParameter<iaColor3f>(IGOR_RESOURCE_PARAM_DIFFUSE, iaColor3f(0.5f, 0.5f, 0.5f));
        _specular = parameters.getParameter<iaColor3f>(IGOR_RESOURCE_PARAM_SPECULAR, iaColor3f(0.6f, 0.6f, 0.6f));
        _shininess = parameters.getParameter<float32>(IGOR_RESOURCE_PARAM_SHININESS, 5.0f);
        _alpha = parameters.getParameter<float32>(IGOR_RESOURCE_PARAM_ALPHA, 1.0f);
        _tiling = parameters.getParameter<iaVector2f>(IGOR_RESOURCE_PARAM_TILING, iaVector2f(1.0f, 1.0f));

        for (int i = 0; i < 4; ++i)
        {
            parameters.hasParameter(IGOR_RESOURCE_PARAM_TEXTURE + iaString::toString(i));
        }
    }

    void iMaterial::setTiling(const iaVector2f &tiling)
    {
        _tiling = tiling;
    }

    const iaVector2f &iMaterial::getTiling() const
    {
        return _tiling;
    }

    void iMaterial::setVelocityOriented(bool enable)
    {
        _velocityOriented = enable;
    }

    bool iMaterial::isVelocityOriented() const
    {
        return _velocityOriented;
    }

    void iMaterial::setTexture(iTexturePtr texture, uint32 texunit)
    {
        _textures[texunit] = texture;
    }

    bool iMaterial::hasTextureUnit(uint32 texunit) const
    {
        auto tex = _textures.find(texunit);
        if (_textures.end() != tex &&
            tex->second != nullptr)
        {
            return true;
        }

        return false;
    }

    iTexturePtr iMaterial::getTexture(uint32 texunit) const
    {
        auto tex = _textures.find(texunit);
        if (_textures.end() != tex)
        {
            return tex->second;
        }

        return nullptr;
    }

    const std::map<uint32, iTexturePtr> &iMaterial::getTextures() const
    {
        return _textures;
    }

    void iMaterial::setEmissive(const iaColor3f &e)
    {
        _emissive = e;
    }

    void iMaterial::setAmbient(const iaColor3f &a)
    {
        _ambient = a;
    }

    void iMaterial::setSpecular(const iaColor3f &s)
    {
        _specular = s;
    }

    void iMaterial::setDiffuse(const iaColor3f &d)
    {
        _diffuse = d;
    }

    void iMaterial::setShininess(float32 shininess)
    {
        _shininess = shininess;
    }

    void iMaterial::setAlpha(float32 alpha)
    {
        _alpha = alpha;
    }

    iaColor3f iMaterial::getAmbient() const
    {
        return _ambient;
    }

    iaColor3f iMaterial::getEmissive() const
    {
        return _emissive;
    }

    iaColor3f iMaterial::getSpecular() const
    {
        return _specular;
    }

    iaColor3f iMaterial::getDiffuse() const
    {
        return _diffuse;
    }

    float32 iMaterial::getShininess() const
    {
        return _shininess;
    }

    float32 iMaterial::getAlpha() const
    {
        return _alpha;
    }

    bool iMaterial::hasTextures() const
    {
        return !_textures.empty();
    }

    void iMaterial::setShaderMaterial(const iShaderMaterialPtr &shaderMaterial)
    {
        _shaderMaterial = shaderMaterial;
    }

    iShaderMaterialPtr iMaterial::getShaderMaterial() const
    {
        return _shaderMaterial;
    }

} // namespace igor