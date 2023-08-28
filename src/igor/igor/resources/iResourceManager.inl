// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

template <> inline
iTexturePtr iResourceManager::requestResource(const iaString &name)
{
    iParameters param({{"name", name}, {"type", iaString("texture")}});
    return std::dynamic_pointer_cast<iTexture>(requestResource(param));
}

template <> inline
iTexturePtr iResourceManager::loadResource(const iaString &name)
{
    iParameters param({{"name", name}, {"type", iaString("texture")}});
    return std::dynamic_pointer_cast<iTexture>(loadResource(param));
}

template <> inline
iSoundPtr iResourceManager::requestResource(const iaString &name)
{
    iParameters param({{"name", name}, {"type", iaString("sound")}});
    return std::dynamic_pointer_cast<iSound>(requestResource(param));
}

template <> inline
iSoundPtr iResourceManager::loadResource(const iaString &name)
{
    iParameters param({{"name", name}, {"type", iaString("sound")}});
    return std::dynamic_pointer_cast<iSound>(loadResource(param));
}

template <> inline
iSpritePtr iResourceManager::requestResource(const iaString &name)
{
    iParameters param({{"name", name}, {"type", iaString("sprite")}});
    return std::dynamic_pointer_cast<iSprite>(requestResource(param));
}

template <> inline
iSpritePtr iResourceManager::loadResource(const iaString &name)
{
    iParameters param({{"name", name}, {"type", iaString("sprite")}});
    return std::dynamic_pointer_cast<iSprite>(loadResource(param));
}

template <> inline
iAnimationPtr iResourceManager::requestResource(const iaString &name)
{
    iParameters param({{"name", name}, {"type", iaString("animation")}});
    return std::dynamic_pointer_cast<iAnimation>(requestResource(param));
}

template <> inline
iAnimationPtr iResourceManager::loadResource(const iaString &name)
{
    iParameters param({{"name", name}, {"type", iaString("animation")}});
    return std::dynamic_pointer_cast<iAnimation>(loadResource(param));
}