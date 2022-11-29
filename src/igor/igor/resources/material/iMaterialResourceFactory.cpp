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
        _defaultMaterial = loadMaterial("default.mat");
        _colorIDMaterial = loadMaterial("colorID.mat");
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

    iMaterialPtr iMaterialResourceFactory::getMaterial(const iaUUID &uuid)
    {
        iMaterialPtr result;

        _mutexMaterial.lock();
        for (auto pair : _materials)
        {
            if (pair.second->getID() == uuid)
            {
                result = pair.second;
                break;
            }
        }
        _mutexMaterial.unlock();

        if (result == nullptr)
        {
            con_err("material with id " << uuid << " does not exist");
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

    iMaterialPtr iMaterialResourceFactory::createMaterial(const iaString &name)
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

        if (nullptr == result.get())
        {
            result = iMaterial::create();
            result->setName(name);
            con_info("loaded material \"" << result->getName() << "\" [" << result->getID() << "]");
            _materials[hashValue] = result;
        }
        _mutexMaterial.unlock();

        return result;
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

        _mutexMaterial.lock();
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
        _mutexMaterial.unlock();

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
        for (auto pair : _materials)
        {
            materials.emplace_back(pair.second);
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
        _mutexMaterial.unlock();
    }

    iTargetMaterialPtr iMaterialResourceFactory::createTargetMaterial()
    {
        iTargetMaterialPtr result = new iTargetMaterial();

        _targetMaterialMutex.lock();
        _targetMaterials.push_back(result);
        _targetMaterialMutex.unlock();

        return result;
    }

    void iMaterialResourceFactory::destroyTargetMaterial(iTargetMaterialPtr targetMaterial)
    {
        _targetMaterialMutex.lock();
        auto iter = std::find(_targetMaterials.begin(), _targetMaterials.end(), targetMaterial);

        if (iter != _targetMaterials.end())
        {
            delete (*iter);
            _targetMaterials.erase(iter);
        }
        _targetMaterialMutex.unlock();
    }

} // namespace igor