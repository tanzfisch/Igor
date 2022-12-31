// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/material/iMaterialResourceFactory.h>

#include <igor/resources/iResourceManager.h>

#include <algorithm>

namespace igor
{
    void iMaterialResourceFactory::init()
    {
        _defaultMaterial = loadMaterial("igor/default.mat");
        _colorIDMaterial = loadMaterial("igor/colorID.mat");
    }

    void iMaterialResourceFactory::deinit()
    {
        _defaultMaterial = nullptr;
        _colorIDMaterial = nullptr;

        flush();

        if (!_materials.empty())
        {
            con_warn("possible mem leak. not all materials were released.");

            if (iaConsole::getInstance().getLogLevel() >= iaLogLevel::Warning)
            {
                con_endl("non released materials: ");
                for (const auto &material : _materials)
                {
                    con_endl(material.second->getID() << " ref:" << material.second.use_count() << " - " << material.second->getName());
                }
            }
        }

        _materials.clear();
    }

    iMaterialPtr iMaterialResourceFactory::getMaterial(const iMaterialID &materialID)
    {
        iMaterialPtr result;

        _mutexMaterial.lock();
        for (const auto &pair : _materials)
        {
            if (pair.second->getID() == materialID)
            {
                result = pair.second;
                break;
            }
        }
        _mutexMaterial.unlock();

        if (result == nullptr)
        {
            con_err("material with id " << materialID << " does not exist");
        }

        return result;
    }

    iMaterialPtr iMaterialResourceFactory::getMaterial(const iaString &name)
    {
        iMaterialPtr result;

        con_assert_sticky(!name.isEmpty(), "empty filename");
        int64 hashValue = name.getHashValue();

        _mutexMaterial.lock();
        auto iter = _materials.find(hashValue);
        if (iter != _materials.end())
        {
            result = iter->second;
        }
        _mutexMaterial.unlock();

        return result;
    }

    iMaterialPtr iMaterialResourceFactory::createMaterial(const iaString &name, bool cache)
    {
        iMaterialPtr result = getMaterial(name);
        if (result != nullptr)
        {
            return result;
        }

        result = iMaterial::create();
        result->setName(name);
        const int64 hashValue = name.getHashValue();

        if (cache)
        {
            _mutexMaterial.lock();
            _materials[hashValue] = result;
            _mutexMaterial.unlock();
        }

        con_info("created material [" << result->getID() << "] \"" << result->getName() << "\"");

        return result;
    }

    bool iMaterialResourceFactory::checkForIDCollisions(iMaterialPtr material)
    {
        bool result = true;

        _mutexMaterial.lock();
        for (const auto &pair : _materials)
        {
            if (material->getID() == pair.second->getID())
            {
                result = false;
                break;
            }
        }
        _mutexMaterial.unlock();

        return result;
    }

    iMaterialPtr iMaterialResourceFactory::loadMaterial(const iaString &filename, bool cache)
    {
        iaString keyPath = iResourceManager::getInstance().getPath(filename);

        if (keyPath.isEmpty())
        {
            keyPath = filename;
        }

        iMaterialPtr result = getMaterial(keyPath);
        if (result != nullptr)
        {
            return result;
        }

        result = iMaterial::create(keyPath);

        con_assert_sticky(checkForIDCollisions(result), "duplicate material ID detected " << result->getID() << " material name:" << result->getName() << " filename:" << filename);

        if (cache)
        {
            const int64 hashValue = keyPath.getHashValue();
            _mutexMaterial.lock();
            _materials[hashValue] = result;
            _mutexMaterial.unlock();
        }

        con_info("loaded material [" << result->getID() << "] \"" << result->getName() << "\" - " << keyPath << " shaders:" << result->getShaderProgram()->getShaderSources().size() << " (" << (cache ? "cached" : "not cached") << ")");

        return result;
    }

    const iMaterialPtr &iMaterialResourceFactory::getDefaultMaterial() const
    {
        return _defaultMaterial;
    }

    const iMaterialPtr &iMaterialResourceFactory::getColorIDMaterial() const
    {
        return _colorIDMaterial;
    }

    void iMaterialResourceFactory::getMaterials(std::vector<iMaterialPtr> &materials)
    {
        materials.clear();

        _mutexMaterial.lock();
        for (const auto &pair : _materials)
        {
            materials.push_back(pair.second);
        }
        _mutexMaterial.unlock();

        sort(materials.begin(), materials.end(),
             [](const iMaterialPtr a, const iMaterialPtr b) -> bool
             {
                 return a->getOrder() < b->getOrder();
             });
    }

    void iMaterialResourceFactory::flush()
    {
        _mutexMaterial.lock();
        auto materialIter = _materials.begin();

        while (materialIter != _materials.end())
        {
            if (materialIter->second.use_count() == 1)
            {
                con_info("released material \"" << materialIter->second->getName() << "\"");
                materialIter = _materials.erase(materialIter);
                continue;
            }

            materialIter++;
        }
        _mutexMaterial.unlock();
    }

} // namespace igor