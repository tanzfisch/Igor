// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/material/iMaterialResourceFactory.h>

#include <igor/resources/iResourceManager.h>

namespace igor
{
    iMaterialResourceFactory::iMaterialResourceFactory()
    {
    }
    iMaterialResourceFactory::~iMaterialResourceFactory()
    {
    }

    void iMaterialResourceFactory::init()
    {
        _defaultMaterial = loadMaterial("default.mat");
    }

    void iMaterialResourceFactory::deinit()
    {
        _defaultMaterial = nullptr;

        flush();

        if (!_materials.empty())
        {
            con_err("possible mem leak. not all materials were released.");
        }

        _materials.clear();
    }

    iMaterialPtr iMaterialResourceFactory::loadMaterial(const iaString &filename)
    {
        iMaterialPtr result;

        con_assert_sticky(!filename.isEmpty(), "empty filename");

        iaString keyPath = iResourceManager::getInstance().getPath(filename);
        if (keyPath.isEmpty())
        {
            keyPath = filename;
        }

        int64 hashValue = filename.getHashValue();

        _mutex.lock();
        auto iter = _materials.find(hashValue);
        if (iter != _materials.end())
        {
            result = iter->second;
        }

        if (nullptr == result.get())
        {
            result = iMaterial::create(keyPath);
            con_info("loaded material \"" << result->getName() << "\" [" << result->getID() << "]");
            _materials[hashValue] = result;
        }
        _mutex.unlock();

        return result;
    }

    const iMaterialPtr &iMaterialResourceFactory::getDefaultMaterial() const
    {
        return _defaultMaterial;
    }

    void iMaterialResourceFactory::flush()
    {
        _mutex.lock();
        auto material = _materials.begin();

        while (material != _materials.end())
        {
            if (material->second.use_count() == 1)
            {
                con_info("released material \"" << material->second->getName() << "\"");
                material = _materials.erase(material);
                continue;
            }

            material++;
        }
        _mutex.unlock();
    }

} // namespace igor