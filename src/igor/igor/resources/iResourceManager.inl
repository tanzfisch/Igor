// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

template <>
inline iTexturePtr iResourceManager::requestResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam("texture" ,alias, cacheMode);
    return std::dynamic_pointer_cast<iTexture>(requestResource(param));
}

template <>
inline iTexturePtr iResourceManager::loadResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam("texture" ,alias, cacheMode);
    return std::dynamic_pointer_cast<iTexture>(loadResource(param));
}

template <>
inline iTexturePtr iResourceManager::requestResource(const iParameters &parameters)
{
    con_assert(parameters.getParameter<iaString>("type", "") == "texture", "incorrect parameter");
    return std::dynamic_pointer_cast<iTexture>(requestResource(parameters));
}

template <>
inline iTexturePtr iResourceManager::loadResource(const iParameters &parameters)
{
    con_assert(parameters.getParameter<iaString>("type", "") == "texture", "incorrect parameter");
    return std::dynamic_pointer_cast<iTexture>(loadResource(parameters));
}

template <>
inline iTexturePtr iResourceManager::getResource(const iResourceID &id)
{
    return std::dynamic_pointer_cast<iTexture>(getResource(id));
}

template <>
inline iTexturePtr iResourceManager::getResource(const iaString &alias)
{
    iParameters param = buildParam("texture" ,alias);
    return std::dynamic_pointer_cast<iTexture>(getResource(param));
}

template <>
inline iSoundPtr iResourceManager::requestResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam("sound" ,alias, cacheMode);
    return std::dynamic_pointer_cast<iSound>(requestResource(param));
}

template <>
inline iSoundPtr iResourceManager::loadResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam("sound" ,alias, cacheMode);
    return std::dynamic_pointer_cast<iSound>(loadResource(param));
}

template <>
inline iSoundPtr iResourceManager::requestResource(const iParameters &parameters)
{
    con_assert(parameters.getParameter<iaString>("type", "") == "sound", "incorrect parameter");
    return std::dynamic_pointer_cast<iSound>(requestResource(parameters));
}

template <>
inline iSoundPtr iResourceManager::loadResource(const iParameters &parameters)
{
    con_assert(parameters.getParameter<iaString>("type", "") == "sound", "incorrect parameter");
    return std::dynamic_pointer_cast<iSound>(loadResource(parameters));
}

template <>
inline iSoundPtr iResourceManager::getResource(const iResourceID &id)
{
    return std::dynamic_pointer_cast<iSound>(getResource(id));
}

template <>
inline iSoundPtr iResourceManager::getResource(const iaString &alias)
{
    iParameters param = buildParam("sound" ,alias);
    return std::dynamic_pointer_cast<iSound>(getResource(param));
}

template <>
inline iSpritePtr iResourceManager::requestResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam("sprite" ,alias, cacheMode);
    return std::dynamic_pointer_cast<iSprite>(requestResource(param));
}

template <>
inline iSpritePtr iResourceManager::loadResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam("sprite" ,alias, cacheMode);
    return std::dynamic_pointer_cast<iSprite>(loadResource(param));
}

template <>
inline iSpritePtr iResourceManager::requestResource(const iParameters &parameters)
{
    con_assert(parameters.getParameter<iaString>("type", "") == "sprite", "incorrect parameter");
    return std::dynamic_pointer_cast<iSprite>(requestResource(parameters));
}

template <>
inline iSpritePtr iResourceManager::loadResource(const iParameters &parameters)
{
    con_assert(parameters.getParameter<iaString>("type", "") == "sprite", "incorrect parameter");
    return std::dynamic_pointer_cast<iSprite>(loadResource(parameters));
}

template <>
inline iSpritePtr iResourceManager::getResource(const iResourceID &id)
{
    return std::dynamic_pointer_cast<iSprite>(getResource(id));
}

template <>
inline iSpritePtr iResourceManager::getResource(const iaString &alias)
{
    iParameters param = buildParam("sprite" ,alias);
    return std::dynamic_pointer_cast<iSprite>(getResource(param));
}

template <>
inline iAnimationPtr iResourceManager::requestResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam("animation" ,alias, cacheMode);
    return std::dynamic_pointer_cast<iAnimation>(requestResource(param));
}

template <>
inline iAnimationPtr iResourceManager::loadResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam("animation" ,alias, cacheMode);
    return std::dynamic_pointer_cast<iAnimation>(loadResource(param));
}

template <>
inline iAnimationPtr iResourceManager::requestResource(const iParameters &parameters)
{
    con_assert(parameters.getParameter<iaString>("type", "") == "animation", "incorrect parameter");
    return std::dynamic_pointer_cast<iAnimation>(requestResource(parameters));
}

template <>
inline iAnimationPtr iResourceManager::loadResource(const iParameters &parameters)
{
    con_assert(parameters.getParameter<iaString>("type", "") == "animation", "incorrect parameter");
    return std::dynamic_pointer_cast<iAnimation>(loadResource(parameters));
}

template <>
inline iAnimationPtr iResourceManager::getResource(const iResourceID &id)
{
    return std::dynamic_pointer_cast<iAnimation>(getResource(id));
}

template <>
inline iAnimationPtr iResourceManager::getResource(const iaString &alias)
{
    iParameters param = buildParam("animation" ,alias);
    return std::dynamic_pointer_cast<iAnimation>(getResource(param));
}

template <>
inline iModelPtr iResourceManager::requestResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam("model" ,alias, cacheMode);
    return std::dynamic_pointer_cast<iModel>(requestResource(param));
}

template <>
inline iModelPtr iResourceManager::loadResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam("model" ,alias, cacheMode);
    return std::dynamic_pointer_cast<iModel>(loadResource(param));
}

template <>
inline iModelPtr iResourceManager::requestResource(const iParameters &parameters)
{
    con_assert(parameters.getParameter<iaString>("type", "") == "model", "incorrect parameter");
    return std::dynamic_pointer_cast<iModel>(requestResource(parameters));
}

template <>
inline iModelPtr iResourceManager::loadResource(const iParameters &parameters)
{
    con_assert(parameters.getParameter<iaString>("type", "") == "model", "incorrect parameter");
    return std::dynamic_pointer_cast<iModel>(loadResource(parameters));
}

template <>
inline iModelPtr iResourceManager::getResource(const iResourceID &id)
{
    return std::dynamic_pointer_cast<iModel>(getResource(id));
}

template <>
inline iModelPtr iResourceManager::getResource(const iaString &alias)
{
    iParameters param = buildParam("model" ,alias);
    return std::dynamic_pointer_cast<iModel>(getResource(param));
}

template <>
inline iMaterialPtr iResourceManager::requestResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam("material" ,alias, cacheMode);
    return std::dynamic_pointer_cast<iMaterial>(requestResource(param));
}

template <>
inline iMaterialPtr iResourceManager::loadResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam("material" ,alias, cacheMode);
    return std::dynamic_pointer_cast<iMaterial>(loadResource(param));
}

template <>
inline iMaterialPtr iResourceManager::requestResource(const iParameters &parameters)
{
    con_assert(parameters.getParameter<iaString>("type", "") == "material", "incorrect parameter");
    return std::dynamic_pointer_cast<iMaterial>(requestResource(parameters));
}

template <>
inline iMaterialPtr iResourceManager::loadResource(const iParameters &parameters)
{
    con_assert(parameters.getParameter<iaString>("type", "") == "material", "incorrect parameter");
    return std::dynamic_pointer_cast<iMaterial>(loadResource(parameters));
}

template <>
inline iMaterialPtr iResourceManager::getResource(const iResourceID &id)
{
    return std::dynamic_pointer_cast<iMaterial>(getResource(id));
}

template <>
inline iMaterialPtr iResourceManager::getResource(const iaString &alias)
{
    iParameters param = buildParam("material" ,alias);
    return std::dynamic_pointer_cast<iMaterial>(getResource(param));
}