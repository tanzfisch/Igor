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
                    con_endl(material.second->getID() << " - " << material.second->getName());
                }
            }
        }

        _materials.clear();
    }

    iMaterialPtr iMaterialResourceFactory::getMaterial(const iMaterialID &materialID)
    {
        iMaterialPtr result;

        con_endl("getMaterial materialID " << materialID);

        std::unordered_map<int64, iMaterialPtr> mycopy;
        _mutexMaterial.lock();
        mycopy.insert(_materials.begin(), _materials.end());
        _mutexMaterial.unlock();

        con_endl(mycopy.size());

        for (const auto &pair : mycopy)
        {
            con_endl(pair.first << " " << pair.second->getName() << " " << pair.second->getID());
        }        

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

    iMaterialPtr iMaterialResourceFactory::createMaterial(const iaString &name)
    {
        iMaterialPtr result = getMaterial(name);
        if (result != nullptr)
        {
            return result;
        }

        result = iMaterial::create();
        result->setName(name); 
        const int64 hashValue = name.getHashValue();

        _mutexMaterial.lock();
        _materials[hashValue] = result;
        _mutexMaterial.unlock();

        con_info("created material [" << result->getID() << "] \"" << result->getName() << "\"");

        return result;
    }

    iMaterialPtr iMaterialResourceFactory::loadMaterial(const iaString &filename)
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
        const int64 hashValue = keyPath.getHashValue();

        _mutexMaterial.lock();
        _materials[hashValue] = result;
        _mutexMaterial.unlock();

        con_info("loaded material [" << result->getID() << "] \"" << result->getName() << "\" - " << keyPath);

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