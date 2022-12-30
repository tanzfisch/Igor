// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/material/iTargetMaterial.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    class iTargetMaterialDeleter
    {
    public:
        void operator()(iTargetMaterial *p) { delete p; }
    };

    iTargetMaterialPtr iTargetMaterial::create()
    {
        return std::shared_ptr<iTargetMaterial>(new iTargetMaterial(), iTargetMaterialDeleter());
    }

    iTargetMaterial::iTargetMaterial()
    {
        _emissive.set(0, 0, 0);
        _ambient.set(0.4f, 0.4f, 0.4f);
        _diffuse.set(0.5f, 0.5f, 0.5f);
        _specular.set(0.6f, 0.6f, 0.6f);
        _shininess = 5.0f;
        _alpha = 1.0f;
        _tilingConfig.set(1.0, 1.0);
    }

    iTargetMaterial::~iTargetMaterial()
    {
    }

    void iTargetMaterial::setTilingConfig(const iaVector2f &tiling)
    {
        _tilingConfig = tiling;
    }

    const iaVector2f &iTargetMaterial::getTilingConfig() const
    {
        return _tilingConfig;
    }

    void iTargetMaterial::setVelocityOriented(bool enable)
    {
        _velocityOriented = enable;
    }

    bool iTargetMaterial::isVelocityOriented() const
    {
        return _velocityOriented;
    }

    void iTargetMaterial::setTexture(iTexturePtr texture, uint32 texunit)
    {
        _textures[texunit] = texture;
    }

    bool iTargetMaterial::hasTextureUnit(uint32 texunit) const
    {
        auto tex = _textures.find(texunit);
        if (_textures.end() != tex &&
            tex->second != nullptr)
        {
            return true;
        }

        return false;
    }

    iTexturePtr iTargetMaterial::getTexture(uint32 texunit) const
    {
        auto tex = _textures.find(texunit);
        if (_textures.end() != tex)
        {
            return tex->second;
        }

        return nullptr;
    }

    const std::map<uint32, iTexturePtr> &iTargetMaterial::getTextures() const
    {
        return _textures;
    }

    void iTargetMaterial::setEmissive(const iaColor3f &e)
    {
        _emissive = e;
    }

    void iTargetMaterial::setAmbient(const iaColor3f &a)
    {
        _ambient = a;
    }

    void iTargetMaterial::setSpecular(const iaColor3f &s)
    {
        _specular = s;
    }

    void iTargetMaterial::setDiffuse(const iaColor3f &d)
    {
        _diffuse = d;
    }

    void iTargetMaterial::setShininess(float32 shininess)
    {
        _shininess = shininess;
    }

    void iTargetMaterial::setAlpha(float32 alpha)
    {
        _alpha = alpha;
    }

    iaColor3f iTargetMaterial::getAmbient() const
    {
        return _ambient;
    }

    iaColor3f iTargetMaterial::getEmissive() const
    {
        return _emissive;
    }

    iaColor3f iTargetMaterial::getSpecular() const
    {
        return _specular;
    }

    iaColor3f iTargetMaterial::getDiffuse() const
    {
        return _diffuse;
    }

    float32 iTargetMaterial::getShininess() const
    {
        return _shininess;
    }

    float32 iTargetMaterial::getAlpha() const
    {
        return _alpha;
    }

    bool iTargetMaterial::hasTextures() const
    {
        return !_textures.empty();
    }

} // namespace igor