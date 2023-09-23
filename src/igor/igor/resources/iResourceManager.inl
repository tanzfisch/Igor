// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

template <>
inline iTexturePtr iResourceManager::requestResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_TEXTURE ,alias, cacheMode);

    iResourcePtr resource = requestResource(param);
    con_assert(resource != nullptr, "failed resource request");
    con_assert(std::dynamic_pointer_cast<iTexture>(resource) != nullptr, "wrong type " << resource->getInfo());

    return std::dynamic_pointer_cast<iTexture>(resource);
}

template <>
inline iTexturePtr iResourceManager::loadResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_TEXTURE ,alias, cacheMode);

    iResourcePtr resource = loadResource(param);
    con_assert(resource != nullptr, "failed resource request");
    con_assert(std::dynamic_pointer_cast<iTexture>(resource) != nullptr, "wrong type " << resource->getInfo());

    return std::dynamic_pointer_cast<iTexture>(resource);
}

template <>
inline iTexturePtr iResourceManager::requestResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_TEXTURE, "incorrect parameter");

    iResourcePtr resource = requestResource(param);
    con_assert(resource != nullptr, "failed resource request");
    con_assert(std::dynamic_pointer_cast<iTexture>(resource) != nullptr, "wrong type " << resource->getInfo());

    return std::dynamic_pointer_cast<iTexture>(resource);
}

template <>
inline iTexturePtr iResourceManager::loadResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_TEXTURE, "incorrect parameter");

    iResourcePtr resource = loadResource(param);
    con_assert(resource != nullptr, "failed resource request");
    con_assert(std::dynamic_pointer_cast<iTexture>(resource) != nullptr, "wrong type " << resource->getInfo());

    return std::dynamic_pointer_cast<iTexture>(resource);
}

template <>
inline iTexturePtr iResourceManager::createResource()
{
    iParameters param(iParametersMap({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_TEXTURE}}));
    iResourcePtr resource = createResource(param);
    con_assert(resource != nullptr, "failed to create resource");
    con_assert(std::dynamic_pointer_cast<iTexture>(resource) != nullptr, "wrong type " << resource->getInfo());
    return std::dynamic_pointer_cast<iTexture>(resource);
}

template <>
inline iTexturePtr iResourceManager::getResource(const iResourceID &id)
{
    return std::dynamic_pointer_cast<iTexture>(getResource(id));
}

template <>
inline iTexturePtr iResourceManager::getResource(const iaString &alias)
{
    iParameters param = buildParam(IGOR_RESOURCE_TEXTURE ,alias);
    return std::dynamic_pointer_cast<iTexture>(getResource(param));
}

template <>
inline iSoundPtr iResourceManager::requestResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_SOUND ,alias, cacheMode);

    iResourcePtr resource = requestResource(param);
    con_assert(resource != nullptr, "failed resource request");
    con_assert(std::dynamic_pointer_cast<iSound>(resource) != nullptr, "wrong type " << resource->getInfo());

    return std::dynamic_pointer_cast<iSound>(resource);
}

template <>
inline iSoundPtr iResourceManager::loadResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_SOUND ,alias, cacheMode);

    iResourcePtr resource = loadResource(param);
    con_assert(resource != nullptr, "failed resource request");
    con_assert(std::dynamic_pointer_cast<iSound>(resource) != nullptr, "wrong type " << resource->getInfo());

    return std::dynamic_pointer_cast<iSound>(resource);
}

template <>
inline iSoundPtr iResourceManager::requestResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_SOUND, "incorrect parameter");

    iResourcePtr resource = requestResource(param);
    con_assert(resource != nullptr, "failed resource request");
    con_assert(std::dynamic_pointer_cast<iSound>(resource) != nullptr, "wrong type " << resource->getInfo());

    return std::dynamic_pointer_cast<iSound>(resource);
}

template <>
inline iSoundPtr iResourceManager::loadResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_SOUND, "incorrect parameter");

    iResourcePtr resource = loadResource(param);
    con_assert(resource != nullptr, "failed resource request");
    con_assert(std::dynamic_pointer_cast<iSound>(resource) != nullptr, "wrong type " << resource->getInfo());

    return std::dynamic_pointer_cast<iSound>(resource);
}

template <>
inline iSoundPtr iResourceManager::createResource()
{
    iParameters param(iParametersMap({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_SOUND}}));
    iResourcePtr resource = createResource(param);
    con_assert(resource != nullptr, "failed to create resource");
    con_assert(std::dynamic_pointer_cast<iSound>(resource) != nullptr, "wrong type " << resource->getInfo());
    return std::dynamic_pointer_cast<iSound>(resource);
}

template <>
inline iSoundPtr iResourceManager::getResource(const iResourceID &id)
{
    return std::dynamic_pointer_cast<iSound>(getResource(id));
}

template <>
inline iSoundPtr iResourceManager::getResource(const iaString &alias)
{
    iParameters param = buildParam(IGOR_RESOURCE_SOUND ,alias);
    return std::dynamic_pointer_cast<iSound>(getResource(param));
}

template <>
inline iSpritePtr iResourceManager::requestResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_SPRITE ,alias, cacheMode);

    iResourcePtr resource = requestResource(param);
    con_assert(resource != nullptr, "failed resource request");
    con_assert(std::dynamic_pointer_cast<iSprite>(resource) != nullptr, "wrong type " << resource->getInfo());

    return std::dynamic_pointer_cast<iSprite>(resource);
}

template <>
inline iSpritePtr iResourceManager::loadResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_SPRITE ,alias, cacheMode);

    iResourcePtr resource = loadResource(param);
    con_assert(resource != nullptr, "failed resource request");
    con_assert(std::dynamic_pointer_cast<iSprite>(resource) != nullptr, "wrong type " << resource->getInfo());

    return std::dynamic_pointer_cast<iSprite>(resource);
}

template <>
inline iSpritePtr iResourceManager::requestResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_SPRITE, "incorrect parameter");

    iResourcePtr resource = requestResource(param);
    con_assert(resource != nullptr, "failed resource request");
    con_assert(std::dynamic_pointer_cast<iSprite>(resource) != nullptr, "wrong type " << resource->getInfo());

    return std::dynamic_pointer_cast<iSprite>(resource);
}

template <>
inline iSpritePtr iResourceManager::loadResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_SPRITE, "incorrect parameter");

    iResourcePtr resource = loadResource(param);
    con_assert(resource != nullptr, "failed resource request");
    con_assert(std::dynamic_pointer_cast<iSprite>(resource) != nullptr, "wrong type " << resource->getInfo());

    return std::dynamic_pointer_cast<iSprite>(resource);
}

template <>
inline iSpritePtr iResourceManager::createResource()
{
    iParameters param(iParametersMap({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_SPRITE}}));
    iResourcePtr resource = createResource(param);
    con_assert(resource != nullptr, "failed to create resource");
    con_assert(std::dynamic_pointer_cast<iSprite>(resource) != nullptr, "wrong type " << resource->getInfo());
    return std::dynamic_pointer_cast<iSprite>(resource);
}

template <>
inline iSpritePtr iResourceManager::getResource(const iResourceID &id)
{
    return std::dynamic_pointer_cast<iSprite>(getResource(id));
}

template <>
inline iSpritePtr iResourceManager::getResource(const iaString &alias)
{
    iParameters param = buildParam(IGOR_RESOURCE_SPRITE ,alias);
    return std::dynamic_pointer_cast<iSprite>(getResource(param));
}

template <>
inline iAnimationPtr iResourceManager::requestResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_ANIMATION ,alias, cacheMode);

    iResourcePtr resource = requestResource(param);
    con_assert(resource != nullptr, "failed resource request");
    con_assert(std::dynamic_pointer_cast<iAnimation>(resource) != nullptr, "wrong type " << resource->getInfo());

    return std::dynamic_pointer_cast<iAnimation>(resource);
}

template <>
inline iAnimationPtr iResourceManager::loadResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_ANIMATION ,alias, cacheMode);

    iResourcePtr resource = loadResource(param);
    con_assert(resource != nullptr, "failed resource request");
    con_assert(std::dynamic_pointer_cast<iAnimation>(resource) != nullptr, "wrong type " << resource->getInfo());

    return std::dynamic_pointer_cast<iAnimation>(resource);
}

template <>
inline iAnimationPtr iResourceManager::requestResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_ANIMATION, "incorrect parameter");

    iResourcePtr resource = requestResource(param);
    con_assert(resource != nullptr, "failed resource request");
    con_assert(std::dynamic_pointer_cast<iAnimation>(resource) != nullptr, "wrong type " << resource->getInfo());

    return std::dynamic_pointer_cast<iAnimation>(resource);
}

template <>
inline iAnimationPtr iResourceManager::loadResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_ANIMATION, "incorrect parameter");

    iResourcePtr resource = loadResource(param);
    con_assert(resource != nullptr, "failed resource request");
    con_assert(std::dynamic_pointer_cast<iAnimation>(resource) != nullptr, "wrong type " << resource->getInfo());

    return std::dynamic_pointer_cast<iAnimation>(resource);
}

template <>
inline iAnimationPtr iResourceManager::createResource()
{
    iParameters param(iParametersMap({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_ANIMATION}}));
    iResourcePtr resource = createResource(param);
    con_assert(resource != nullptr, "failed to create resource");
    con_assert(std::dynamic_pointer_cast<iAnimation>(resource) != nullptr, "wrong type " << resource->getInfo());
    return std::dynamic_pointer_cast<iAnimation>(resource);
}

template <>
inline iAnimationPtr iResourceManager::getResource(const iResourceID &id)
{
    return std::dynamic_pointer_cast<iAnimation>(getResource(id));
}

template <>
inline iAnimationPtr iResourceManager::getResource(const iaString &alias)
{
    iParameters param = buildParam(IGOR_RESOURCE_ANIMATION ,alias);
    return std::dynamic_pointer_cast<iAnimation>(getResource(param));
}

template <>
inline iModelPtr iResourceManager::requestResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_MODEL ,alias, cacheMode);

    iResourcePtr resource = requestResource(param);
    con_assert(resource != nullptr, "failed resource request");
    con_assert(std::dynamic_pointer_cast<iModel>(resource) != nullptr, "wrong type " << resource->getInfo());

    return std::dynamic_pointer_cast<iModel>(resource);
}

template <>
inline iModelPtr iResourceManager::loadResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_MODEL ,alias, cacheMode);

    iResourcePtr resource = loadResource(param);
    con_assert(resource != nullptr, "failed resource request");
    con_assert(std::dynamic_pointer_cast<iModel>(resource) != nullptr, "wrong type " << resource->getInfo());

    return std::dynamic_pointer_cast<iModel>(resource);
}

template <>
inline iModelPtr iResourceManager::requestResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_MODEL, "incorrect parameter");

    iResourcePtr resource = requestResource(param);
    con_assert(resource != nullptr, "failed resource request");
    con_assert(std::dynamic_pointer_cast<iModel>(resource) != nullptr, "wrong type " << resource->getInfo());

    return std::dynamic_pointer_cast<iModel>(resource);
}

template <>
inline iModelPtr iResourceManager::loadResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_MODEL, "incorrect parameter");

    iResourcePtr resource = loadResource(param);
    con_assert(resource != nullptr, "failed resource request");
    con_assert(std::dynamic_pointer_cast<iModel>(resource) != nullptr, "wrong type " << resource->getInfo());

    return std::dynamic_pointer_cast<iModel>(resource);
}

template <>
inline iModelPtr iResourceManager::createResource()
{
    iParameters param(iParametersMap({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MODEL}}));
    iResourcePtr resource = createResource(param);
    con_assert(resource != nullptr, "failed to create resource");
    con_assert(std::dynamic_pointer_cast<iModel>(resource) != nullptr, "wrong type " << resource->getInfo());
    return std::dynamic_pointer_cast<iModel>(resource);
}

template <>
inline iModelPtr iResourceManager::getResource(const iResourceID &id)
{
    return std::dynamic_pointer_cast<iModel>(getResource(id));
}

template <>
inline iModelPtr iResourceManager::getResource(const iaString &alias)
{
    iParameters param = buildParam(IGOR_RESOURCE_MODEL ,alias);
    return std::dynamic_pointer_cast<iModel>(getResource(param));
}

template <>
inline iMaterialPtr iResourceManager::requestResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_MATERIAL ,alias, cacheMode);

    iResourcePtr resource = requestResource(param);
    con_assert(resource != nullptr, "failed resource request");
    con_assert(std::dynamic_pointer_cast<iMaterial>(resource) != nullptr, "wrong type " << resource->getInfo());

    return std::dynamic_pointer_cast<iMaterial>(resource);
}

template <>
inline iMaterialPtr iResourceManager::loadResource(const iaString &alias, iResourceCacheMode cacheMode)
{
    iParameters param = buildParam(IGOR_RESOURCE_MATERIAL ,alias, cacheMode);

    iResourcePtr resource = loadResource(param);
    con_assert(resource != nullptr, "failed resource request");
    con_assert(std::dynamic_pointer_cast<iMaterial>(resource) != nullptr, "wrong type " << resource->getInfo());

    return std::dynamic_pointer_cast<iMaterial>(resource);
}

template <>
inline iMaterialPtr iResourceManager::requestResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_MATERIAL, "incorrect parameter");

    iResourcePtr resource = requestResource(param);
    con_assert(resource != nullptr, "failed resource request");
    con_assert(std::dynamic_pointer_cast<iMaterial>(resource) != nullptr, "wrong type " << resource->getInfo());

    return std::dynamic_pointer_cast<iMaterial>(resource);
}

template <>
inline iMaterialPtr iResourceManager::loadResource(const iParameters &param)
{
    con_assert(param.getParameter<iaString>(IGOR_RESOURCE_PARAM_TYPE, "") == IGOR_RESOURCE_MATERIAL, "incorrect parameter");

    iResourcePtr resource = loadResource(param);
    con_assert(resource != nullptr, "failed resource request");
    con_assert(std::dynamic_pointer_cast<iMaterial>(resource) != nullptr, "wrong type " << resource->getInfo());

    return std::dynamic_pointer_cast<iMaterial>(resource);
}

template <>
inline iMaterialPtr iResourceManager::createResource()
{
    iParameters param(iParametersMap({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL}}));
    iResourcePtr resource = createResource(param);
    con_assert(resource != nullptr, "failed to create resource");
    con_assert(std::dynamic_pointer_cast<iMaterial>(resource) != nullptr, "wrong type " << resource->getInfo());
    return std::dynamic_pointer_cast<iMaterial>(resource);
}

template <>
inline iMaterialPtr iResourceManager::getResource(const iResourceID &id)
{
    return std::dynamic_pointer_cast<iMaterial>(getResource(id));
}

template <>
inline iMaterialPtr iResourceManager::getResource(const iaString &alias)
{
    iParameters param = buildParam(IGOR_RESOURCE_MATERIAL ,alias);
    return std::dynamic_pointer_cast<iMaterial>(getResource(param));
}