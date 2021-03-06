// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/material/iTargetMaterial.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iTargetMaterial::iTargetMaterial()
    {
        _emissive.set(0, 0, 0);
        _ambient.set(0.4f, 0.4f, 0.4f);
        _diffuse.set(0.5f, 0.5f, 0.5f);
        _specular.set(0.6f, 0.6f, 0.6f);
        _shininess = 5.0f;
        _alpha = 1.0f;
    }

    iTargetMaterial::~iTargetMaterial()
    {
    }

    void iTargetMaterial::setTexture(iTexturePtr texture, int texunit)
    {
        con_assert(texunit < 4 && texunit >= 0, "tex unit out of range");
        _textures[texunit] = texture;
    }

    iTexturePtr iTargetMaterial::getTexture(int texunit) const
    {
        con_assert(_textures.size() != 0, "no textures available");

        auto tex = _textures.find(texunit);
        if (_textures.end() != tex)
        {
            return tex->second;
        }

        return nullptr;
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

    void iTargetMaterial::setShininess(float32 s)
    {
        _shininess = s;
    }

    void iTargetMaterial::setAlpha(float32 alpha)
    {
        _alpha = alpha;
    }
} // namespace igor