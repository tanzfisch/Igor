// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

template <>
inline iTexturePtr iResourceManager::requestResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_TEXTURE, alias, cacheMode);
    return std::dynamic_pointer_cast<iTexture>(requestResource(param));
}

template <>
inline iTexturePtr iResourceManager::loadResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_TEXTURE, alias, cacheMode);
    return std::dynamic_pointer_cast<iTexture>(loadResource(param));
}

template <>
inline iTexturePtr iResourceManager::requestResource(const iResourceID &resourceID, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_TEXTURE, resourceID, cacheMode);
    return std::dynamic_pointer_cast<iTexture>(requestResource(param));
}

template <>
inline iTexturePtr iResourceManager::loadResource(const iResourceID &resourceID, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_TEXTURE, resourceID, cacheMode);
    return std::dynamic_pointer_cast<iTexture>(loadResource(param));
}

template <>
inline iTexturePtr iResourceManager::requestResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_TEXTURE, "incorrect parameter");
    return std::dynamic_pointer_cast<iTexture>(requestResource(param));
}

template <>
inline iTexturePtr iResourceManager::loadResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_TEXTURE, "incorrect parameter");
    return std::dynamic_pointer_cast<iTexture>(loadResource(param));
}

template <>
inline iTexturePtr iResourceManager::createResource()
{
    iParameters param(iParametersMap({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_TEXTURE}}));
    return std::dynamic_pointer_cast<iTexture>(createResource(param));
}

template <>
inline iTexturePtr iResourceManager::getResource(const iResourceID &id)
{
    return std::dynamic_pointer_cast<iTexture>(getResource(id));
}

template <>
inline iTexturePtr iResourceManager::getResource(const iaString &alias)
{
    iParameters param = buildParam(IGOR_RESOURCE_TEXTURE, alias);
    return std::dynamic_pointer_cast<iTexture>(getResource(param));
}

template <>
inline iSoundPtr iResourceManager::requestResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_SOUND, alias, cacheMode);
    return std::dynamic_pointer_cast<iSound>(requestResource(param));
}

template <>
inline iSoundPtr iResourceManager::loadResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_SOUND, alias, cacheMode);
    return std::dynamic_pointer_cast<iSound>(loadResource(param));
}

template <>
inline iSoundPtr iResourceManager::requestResource(const iResourceID &resourceID, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_SOUND, resourceID, cacheMode);
    return std::dynamic_pointer_cast<iSound>(requestResource(param));
}

template <>
inline iSoundPtr iResourceManager::loadResource(const iResourceID &resourceID, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_SOUND, resourceID, cacheMode);
    return std::dynamic_pointer_cast<iSound>(loadResource(param));
}

template <>
inline iSoundPtr iResourceManager::requestResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_SOUND, "incorrect parameter");
    return std::dynamic_pointer_cast<iSound>(requestResource(param));
}

template <>
inline iSoundPtr iResourceManager::loadResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_SOUND, "incorrect parameter");
    return std::dynamic_pointer_cast<iSound>(loadResource(param));
}

template <>
inline iSoundPtr iResourceManager::createResource()
{
    iParameters param(iParametersMap({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_SOUND}}));
    return std::dynamic_pointer_cast<iSound>(createResource(param));
}

template <>
inline iSoundPtr iResourceManager::getResource(const iResourceID &id)
{
    return std::dynamic_pointer_cast<iSound>(getResource(id));
}

template <>
inline iSoundPtr iResourceManager::getResource(const iaString &alias)
{
    iParameters param = buildParam(IGOR_RESOURCE_SOUND, alias);
    return std::dynamic_pointer_cast<iSound>(getResource(param));
}

template <>
inline iSpritePtr iResourceManager::requestResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_SPRITE, alias, cacheMode);
    return std::dynamic_pointer_cast<iSprite>(requestResource(param));
}

template <>
inline iSpritePtr iResourceManager::loadResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_SPRITE, alias, cacheMode);
    return std::dynamic_pointer_cast<iSprite>(loadResource(param));
}

template <>
inline iSpritePtr iResourceManager::requestResource(const iResourceID &resourceID, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_SPRITE, resourceID, cacheMode);
    return std::dynamic_pointer_cast<iSprite>(requestResource(param));
}

template <>
inline iSpritePtr iResourceManager::loadResource(const iResourceID &resourceID, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_SPRITE, resourceID, cacheMode);
    return std::dynamic_pointer_cast<iSprite>(loadResource(param));
}

template <>
inline iSpritePtr iResourceManager::requestResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_SPRITE, "incorrect parameter");
    return std::dynamic_pointer_cast<iSprite>(requestResource(param));
}

template <>
inline iSpritePtr iResourceManager::loadResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_SPRITE, "incorrect parameter");
    return std::dynamic_pointer_cast<iSprite>(loadResource(param));
}

template <>
inline iSpritePtr iResourceManager::createResource()
{
    iParameters param(iParametersMap({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_SPRITE}}));
    return std::dynamic_pointer_cast<iSprite>(createResource(param));
}

template <>
inline iSpritePtr iResourceManager::getResource(const iResourceID &id)
{
    return std::dynamic_pointer_cast<iSprite>(getResource(id));
}

template <>
inline iSpritePtr iResourceManager::getResource(const iaString &alias)
{
    iParameters param = buildParam(IGOR_RESOURCE_SPRITE, alias);
    return std::dynamic_pointer_cast<iSprite>(getResource(param));
}

template <>
inline iAnimationPtr iResourceManager::requestResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_ANIMATION, alias, cacheMode);
    return std::dynamic_pointer_cast<iAnimation>(requestResource(param));
}

template <>
inline iAnimationPtr iResourceManager::loadResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_ANIMATION, alias, cacheMode);
    return std::dynamic_pointer_cast<iAnimation>(loadResource(param));
}

template <>
inline iAnimationPtr iResourceManager::requestResource(const iResourceID &resourceID, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_ANIMATION, resourceID, cacheMode);
    return std::dynamic_pointer_cast<iAnimation>(requestResource(param));
}

template <>
inline iAnimationPtr iResourceManager::loadResource(const iResourceID &resourceID, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_ANIMATION, resourceID, cacheMode);
    return std::dynamic_pointer_cast<iAnimation>(loadResource(param));
}

template <>
inline iAnimationPtr iResourceManager::requestResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_ANIMATION, "incorrect parameter");
    return std::dynamic_pointer_cast<iAnimation>(requestResource(param));
}

template <>
inline iAnimationPtr iResourceManager::loadResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_ANIMATION, "incorrect parameter");
    return std::dynamic_pointer_cast<iAnimation>(loadResource(param));
}

template <>
inline iAnimationPtr iResourceManager::createResource()
{
    iParameters param(iParametersMap({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_ANIMATION}}));
    return std::dynamic_pointer_cast<iAnimation>(createResource(param));
}

template <>
inline iAnimationPtr iResourceManager::getResource(const iResourceID &id)
{
    return std::dynamic_pointer_cast<iAnimation>(getResource(id));
}

template <>
inline iAnimationPtr iResourceManager::getResource(const iaString &alias)
{
    iParameters param = buildParam(IGOR_RESOURCE_ANIMATION, alias);
    return std::dynamic_pointer_cast<iAnimation>(getResource(param));
}

template <>
inline iModelPtr iResourceManager::requestResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_MODEL, alias, cacheMode);
    return std::dynamic_pointer_cast<iModel>(requestResource(param));
}

template <>
inline iModelPtr iResourceManager::loadResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_MODEL, alias, cacheMode);
    return std::dynamic_pointer_cast<iModel>(loadResource(param));
}

template <>
inline iModelPtr iResourceManager::requestResource(const iResourceID &resourceID, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_MODEL, resourceID, cacheMode);
    return std::dynamic_pointer_cast<iModel>(requestResource(param));
}

template <>
inline iModelPtr iResourceManager::loadResource(const iResourceID &resourceID, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_MODEL, resourceID, cacheMode);
    return std::dynamic_pointer_cast<iModel>(loadResource(param));
}

template <>
inline iModelPtr iResourceManager::requestResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_MODEL, "incorrect parameter");
    return std::dynamic_pointer_cast<iModel>(requestResource(param));
}

template <>
inline iModelPtr iResourceManager::loadResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_MODEL, "incorrect parameter");
    return std::dynamic_pointer_cast<iModel>(loadResource(param));
}

template <>
inline iModelPtr iResourceManager::createResource()
{
    iParameters param(iParametersMap({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MODEL}}));
    return std::dynamic_pointer_cast<iModel>(createResource(param));
}

template <>
inline iModelPtr iResourceManager::getResource(const iResourceID &id)
{
    return std::dynamic_pointer_cast<iModel>(getResource(id));
}

template <>
inline iModelPtr iResourceManager::getResource(const iaString &alias)
{
    iParameters param = buildParam(IGOR_RESOURCE_MODEL, alias);
    return std::dynamic_pointer_cast<iModel>(getResource(param));
}

template <>
inline iShaderPtr iResourceManager::requestResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_SHADER, alias, cacheMode);
    return std::dynamic_pointer_cast<iShader>(requestResource(param));
}

template <>
inline iShaderPtr iResourceManager::loadResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_SHADER, alias, cacheMode);
    return std::dynamic_pointer_cast<iShader>(loadResource(param));
}

template <>
inline iShaderPtr iResourceManager::requestResource(const iResourceID &resourceID, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_SHADER, resourceID, cacheMode);
    return std::dynamic_pointer_cast<iShader>(requestResource(param));
}

template <>
inline iShaderPtr iResourceManager::loadResource(const iResourceID &resourceID, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_SHADER, resourceID, cacheMode);
    return std::dynamic_pointer_cast<iShader>(loadResource(param));
}

template <>
inline iShaderPtr iResourceManager::requestResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_SHADER, "incorrect parameter");
    return std::dynamic_pointer_cast<iShader>(requestResource(param));
}

template <>
inline iShaderPtr iResourceManager::loadResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_SHADER, "incorrect parameter");
    return std::dynamic_pointer_cast<iShader>(loadResource(param));
}

template <>
inline iShaderPtr iResourceManager::createResource()
{
    iParameters param(iParametersMap({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_SHADER}}));
    return std::dynamic_pointer_cast<iShader>(createResource(param));
}

template <>
inline iShaderPtr iResourceManager::getResource(const iResourceID &id)
{
    return std::dynamic_pointer_cast<iShader>(getResource(id));
}

template <>
inline iShaderPtr iResourceManager::getResource(const iaString &alias)
{
    iParameters param = buildParam(IGOR_RESOURCE_SHADER, alias);
    return std::dynamic_pointer_cast<iShader>(getResource(param));
}

template <>
inline iMaterialPtr iResourceManager::requestResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_MATERIAL, alias, cacheMode);
    return std::dynamic_pointer_cast<iMaterial>(requestResource(param));
}

template <>
inline iMaterialPtr iResourceManager::loadResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_MATERIAL, alias, cacheMode);
    return std::dynamic_pointer_cast<iMaterial>(loadResource(param));
}

template <>
inline iMaterialPtr iResourceManager::requestResource(const iResourceID &resourceID, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_MATERIAL, resourceID, cacheMode);
    return std::dynamic_pointer_cast<iMaterial>(requestResource(param));
}

template <>
inline iMaterialPtr iResourceManager::loadResource(const iResourceID &resourceID, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_MATERIAL, resourceID, cacheMode);
    return std::dynamic_pointer_cast<iMaterial>(loadResource(param));
}

template <>
inline iMaterialPtr iResourceManager::requestResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_MATERIAL, "incorrect parameter");
    return std::dynamic_pointer_cast<iMaterial>(requestResource(param));
}

template <>
inline iMaterialPtr iResourceManager::loadResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_MATERIAL, "incorrect parameter");
    return std::dynamic_pointer_cast<iMaterial>(loadResource(param));
}

template <>
inline iMaterialPtr iResourceManager::createResource()
{
    iParameters param(iParametersMap({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL}}));
    return std::dynamic_pointer_cast<iMaterial>(createResource(param));
}

template <>
inline iMaterialPtr iResourceManager::getResource(const iResourceID &id)
{
    return std::dynamic_pointer_cast<iMaterial>(getResource(id));
}

template <>
inline iMaterialPtr iResourceManager::getResource(const iaString &alias)
{
    iParameters param = buildParam(IGOR_RESOURCE_MATERIAL, alias);
    return std::dynamic_pointer_cast<iMaterial>(getResource(param));
}

template <>
inline iPrefabPtr iResourceManager::requestResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_PREFAB, alias, cacheMode);
    return std::dynamic_pointer_cast<iPrefab>(requestResource(param));
}

template <>
inline iPrefabPtr iResourceManager::loadResource(const iaString &alias, iResourceCacheMode cacheMode)
{
     iParameters param = buildParam(IGOR_RESOURCE_PREFAB, alias, cacheMode);
    return std::dynamic_pointer_cast<iPrefab>(loadResource(param));
}

template <>
inline iPrefabPtr iResourceManager::requestResource(const iResourceID &resourceID, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_PREFAB, resourceID, cacheMode);
    // TODO force loadResource for prefabs until we can handle parallel loading
    return std::dynamic_pointer_cast<iPrefab>(loadResource(param));
}

template <>
inline iPrefabPtr iResourceManager::loadResource(const iResourceID &resourceID, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_PREFAB, resourceID, cacheMode);
    return std::dynamic_pointer_cast<iPrefab>(loadResource(param));
}

template <>
inline iPrefabPtr iResourceManager::requestResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_PREFAB, "incorrect parameter");
    return std::dynamic_pointer_cast<iPrefab>(requestResource(param));
}

template <>
inline iPrefabPtr iResourceManager::loadResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_PREFAB, "incorrect parameter");
    return std::dynamic_pointer_cast<iPrefab>(loadResource(param));
}

template <>
inline iPrefabPtr iResourceManager::createResource()
{
    iParameters param(iParametersMap({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_PREFAB}}));
    return std::dynamic_pointer_cast<iPrefab>(createResource(param));
}

template <>
inline iPrefabPtr iResourceManager::getResource(const iResourceID &id)
{
    return std::dynamic_pointer_cast<iPrefab>(getResource(id));
}

template <>
inline iPrefabPtr iResourceManager::getResource(const iaString &alias)
{
    iParameters param = buildParam(IGOR_RESOURCE_PREFAB, alias);
    return std::dynamic_pointer_cast<iPrefab>(getResource(param));
}