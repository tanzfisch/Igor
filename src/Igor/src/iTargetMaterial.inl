// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

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

bool iTargetMaterial::hasTextures() const
{
    return _textures.size() > 0 ? true : false;
}

bool iTargetMaterial::hasTextureUnit(uint32 unit) const
{
    auto iter = _textures.find(unit);
    if (iter != _textures.end())
    {
        if ((*iter).second != nullptr)
        {
            return true;
        }
    }

    return false;
}

uint32 iTargetMaterial::getTextureUnitCount() const
{
    return static_cast<uint32>(_textures.size());
}
