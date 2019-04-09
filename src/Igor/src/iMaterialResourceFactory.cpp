// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iMaterialResourceFactory.h>

#include <iMaterial.h>
#include <iShader.h>
#include <iRenderer.h>
#include <iTargetMaterial.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iMaterialResourceFactory::iMaterialResourceFactory()
    {
        iRenderer::getInstance().registerInitializedDelegate(iRendererInitializedDelegate(this, &iMaterialResourceFactory::initDefaultMaterials));

        // if already ready just use it now
        if (iRenderer::getInstance().isReady())
        {
            initDefaultMaterials();
        }
    }

    iMaterialResourceFactory::~iMaterialResourceFactory()
    {
        iRenderer::getInstance().unregisterInitializedDelegate(iRendererInitializedDelegate(this, &iMaterialResourceFactory::initDefaultMaterials));

        if (_defaultMaterial != iMaterial::INVALID_MATERIAL_ID)
        {
            destroyMaterial(_defaultMaterial);
        }

        if (!_materials.empty())
        {
            con_debug_endl(static_cast<int>(_materials.size()) << " materials left. will clean up for you");
        }

        _materials.clear();
        _sortedMaterials.clear();
        _currentMaterial = nullptr;

        if (!_targetMaterials.empty())
        {
            con_err("possible mem leak! " << _targetMaterials.size() << " target materials left");
        }

        auto targetmaterialIter = _targetMaterials.begin();
        while (targetmaterialIter != _targetMaterials.end())
        {
            delete (*targetmaterialIter);
            ++targetmaterialIter;
        }

        _targetMaterials.clear();
    }

    iTargetMaterial* iMaterialResourceFactory::createTargetMaterial()
    {
        iTargetMaterial* result = new iTargetMaterial();

        _targetMaterialMutex.lock();
        _targetMaterials.push_back(result);
        _targetMaterialMutex.unlock();

        return result;
    }

    void iMaterialResourceFactory::destroyTargetMaterial(iTargetMaterial* targetMaterial)
    {
        _targetMaterialMutex.lock();
        auto iter = find(_targetMaterials.begin(), _targetMaterials.end(), targetMaterial);

        if (iter != _targetMaterials.end())
        {
            delete (*iter);
            _targetMaterials.erase(iter);
        }
        _targetMaterialMutex.unlock();
    }

    uint64 iMaterialResourceFactory::getDefaultMaterialID() const
    {
        return _defaultMaterial;
    }

    uint64 iMaterialResourceFactory::getColorIDMaterialID() const
    {
        return _colorIDMaterial;
    }

    void iMaterialResourceFactory::updateGroups()
    {
        _mutexMaterial.lock();
        if (_dirtyMaterials)
        {
            sort(_sortedMaterials.begin(), _sortedMaterials.end(),
                [](const iMaterialPtr a, const iMaterialPtr b) -> bool
            {
                return a->getOrder() < b->getOrder();
            });

            _dirtyMaterials = false;
        }
        _mutexMaterial.unlock();
    }

    vector<iMaterialPtr> iMaterialResourceFactory::getSortedMaterials()
    {
        updateGroups();

        _mutexMaterial.lock();
        vector<iMaterialPtr> copyList(_sortedMaterials);
        _mutexMaterial.unlock();

        return copyList;
    }

    void iMaterialResourceFactory::initDefaultMaterials()
    {
        // create the default material
        _defaultMaterial = createMaterial();
        auto material = getMaterial(_defaultMaterial);
        material->setName("IgorDefault");
        material->addShaderSource("igor/default.vert", iShaderObjectType::Vertex);
        material->addShaderSource("igor/default_directional_light.frag", iShaderObjectType::Fragment);
        material->compileShader();
        material->setOrder(iMaterial::RENDER_ORDER_DEFAULT);

        // set material to start with
        setMaterial(_defaultMaterial);

        // create the color ID material
        _colorIDMaterial = createMaterial();
        material = getMaterial(_colorIDMaterial);
        material->setName("IgorColorID");
        material->addShaderSource("igor/default.vert", iShaderObjectType::Vertex);
        material->addShaderSource("igor/solidColor.frag", iShaderObjectType::Fragment);
        material->compileShader();
        material->setOrder(iMaterial::RENDER_ORDER_DEFAULT);
    }

    uint64 iMaterialResourceFactory::createMaterial(iaString name)
    {
        iMaterial* material = new iMaterial();
        if (name != L"")
        {
            material->setName(name);
        }

        material->_isValid = true;

        iMaterialPtr shared = iMaterialPtr(material, iMaterial::private_deleter());

        _mutexMaterial.lock();
        _materials[material->getID()] = shared;
        _sortedMaterials.push_back(shared);
        _dirtyMaterials = true;
        _mutexMaterial.unlock();

        _materialCreatedEvent(material->getID());

        return material->getID();
    }

    void iMaterialResourceFactory::registerMaterialCreatedDelegate(iMaterialCreatedDelegate materialCreatedDelegate)
    {
        _materialCreatedEvent.append(materialCreatedDelegate);
    }

    void iMaterialResourceFactory::unregisterMaterialCreatedDelegate(iMaterialCreatedDelegate materialCreatedDelegate)
    {
        _materialCreatedEvent.remove(materialCreatedDelegate);
    }

    void iMaterialResourceFactory::registerMaterialDestroyedDelegate(iMaterialDestroyedDelegate materialDestroyedDelegate)
    {
        _materialDestroyedEvent.append(materialDestroyedDelegate);
    }

    void iMaterialResourceFactory::unregisterMaterialDestroyedDelegate(iMaterialDestroyedDelegate materialDestroyedDelegate)
    {
        _materialDestroyedEvent.remove(materialDestroyedDelegate);
    }

    void iMaterialResourceFactory::destroyMaterial(uint64 materialID)
    {
        _mutexMaterial.lock();

        auto iterMaterial = _materials.find(materialID);
        if (iterMaterial != _materials.end())
        {
            (*iterMaterial).second->_isValid = false;

            auto sortedIter = find(_sortedMaterials.begin(), _sortedMaterials.end(), (*iterMaterial).second);

            con_assert_sticky(sortedIter != _sortedMaterials.end(), "inconsistent material list");

            if (sortedIter != _sortedMaterials.end())
            {
                _sortedMaterials.erase(sortedIter);
            }

            _materials.erase(iterMaterial);
        }
        else
        {
            con_err("material id: " << materialID << " does not exist");
        }

        _mutexMaterial.unlock();

        _materialDestroyedEvent(materialID);
    }

    void iMaterialResourceFactory::setMaterial(uint64 materialID)
    {
        iMaterialPtr material = 0;

        _mutexMaterial.lock();
        auto iter = _materials.find(materialID);
        if (iter != _materials.end())
        {
            material = (*iter).second;
        }
        _mutexMaterial.unlock();

        if (material != nullptr)
        {
            con_assert_sticky(iRenderer::getInstance().isReady(), "renderer not ready");
            iRenderer::getInstance().setMaterial(material);
        }
    }

    iMaterialPtr iMaterialResourceFactory::getDefaultMaterial()
    {
        return getMaterial(_defaultMaterial);
    }

    iMaterialPtr iMaterialResourceFactory::getMaterial(uint64 materialID)
    {
        iMaterialPtr material = nullptr;

        _mutexMaterial.lock();
        auto iter = _materials.find(materialID);
        if (iter != _materials.end())
        {
            material = (*iter).second;
        }
        _mutexMaterial.unlock();

        if (material == nullptr)
        {
            con_err("material id: " << materialID << " does not exist");
        }

        return material;
    }

    uint64 iMaterialResourceFactory::getMaterialID(iaString materialName)
    {
        uint64 result = iMaterial::INVALID_MATERIAL_ID;

        _mutexMaterial.lock();
        auto materialIter = _materials.begin();
        while (materialIter != _materials.end())
        {
            if ((*materialIter).second->getName() == materialName)
            {
                result = (*materialIter).first;
            }

            ++materialIter;
        }

        _mutexMaterial.unlock();

        if (result == iMaterial::INVALID_MATERIAL_ID)
        {
            con_err("material with name:" << materialName << " does not exist");
        }

        return result;
    }

    iMaterialPtr iMaterialResourceFactory::getMaterial(iaString materialName)
    {
        iMaterialPtr material = 0;

        _mutexMaterial.lock();
        auto materialIter = _materials.begin();
        while (materialIter != _materials.end())
        {
            if ((*materialIter).second->getName() == materialName)
            {
                material = (*materialIter).second;
                break;
            }

            ++materialIter;
        }

        _mutexMaterial.unlock();

        if (material == nullptr)
        {
            con_err("material with name:" << materialName << " does not exist");
        }

        return material;
    }

    iMaterialPtr iMaterialResourceFactory::getCurrentMaterial()
    {
        return _currentMaterial;
    }

}