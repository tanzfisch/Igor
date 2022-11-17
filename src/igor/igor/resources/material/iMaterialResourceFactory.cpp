// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/material/iMaterialResourceFactory.h>

#include <igor/resources/material/iMaterial.h>
#include <igor/resources/material/iShader_Old.h>
#include <igor/renderer/iRenderer.h>
#include <igor/resources/material/iTargetMaterial.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
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

        if (_defaultMaterial != iMaterial_old::INVALID_MATERIAL_ID)
        {
            destroyMaterial(_defaultMaterial);
        }

        if (!_materials.empty())
        {
            con_debug(static_cast<int>(_materials.size()) << " materials left. will clean up for you");
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

    iTargetMaterial *iMaterialResourceFactory::createTargetMaterial()
    {
        iTargetMaterial *result = new iTargetMaterial();

        _targetMaterialMutex.lock();
        _targetMaterials.push_back(result);
        _targetMaterialMutex.unlock();

        return result;
    }

    void iMaterialResourceFactory::destroyTargetMaterial(iTargetMaterial *targetMaterial)
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
                 [](const iMaterial_oldPtr a, const iMaterial_oldPtr b) -> bool {
                     return a->getOrder() < b->getOrder();
                 });

            _dirtyMaterials = false;
        }
        _mutexMaterial.unlock();
    }

    std::vector<iMaterial_oldPtr> iMaterialResourceFactory::getSortedMaterials()
    {
        updateGroups();

        _mutexMaterial.lock();
        std::vector<iMaterial_oldPtr> copyList(_sortedMaterials);
        _mutexMaterial.unlock();

        return copyList;
    }

    void iMaterialResourceFactory::initDefaultMaterials()
    {
        // create the default material
        _defaultMaterial = createMaterial("igor.default");
        auto material = getMaterial(_defaultMaterial);
        material->addShaderSource("igor/default.vert", iShaderObjectType::Vertex);
        material->addShaderSource("igor/default_directional_light.frag", iShaderObjectType::Fragment);
        material->compileShader();
        material->setOrder(iMaterial_old::RENDER_ORDER_DEFAULT);

        // set material to start with
        setMaterial(_defaultMaterial);

        // create the color ID material
        _colorIDMaterial = createMaterial("igor.colorid");
        material = getMaterial(_colorIDMaterial);
        material->addShaderSource("igor/default.vert", iShaderObjectType::Vertex);
        material->addShaderSource("igor/solidColor.frag", iShaderObjectType::Fragment);
        material->compileShader();
        material->setOrder(iMaterial_old::RENDER_ORDER_DEFAULT);

        // creating this one as option because it is used far more often then the actual default one
        uint64 defaultTextureMaterial = createMaterial("igor.texture");
        material = getMaterial(defaultTextureMaterial);
        material->addShaderSource("igor/textured.vert", iShaderObjectType::Vertex);
        material->addShaderSource("igor/textured_directional_light.frag", iShaderObjectType::Fragment);
        material->compileShader();
        material->setOrder(iMaterial_old::RENDER_ORDER_DEFAULT);
    }

    uint64 iMaterialResourceFactory::createMaterial(iaString name)
    {
        iMaterial_old *material = new iMaterial_old();
        if (name != L"")
        {
            material->setName(name);
        }

        material->_isValid = true;

        iMaterial_oldPtr shared = iMaterial_oldPtr(material);

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
        iMaterial_oldPtr material = 0;

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

    iMaterial_oldPtr iMaterialResourceFactory::getDefaultMaterial()
    {
        return getMaterial(_defaultMaterial);
    }

    iMaterial_oldPtr iMaterialResourceFactory::getMaterial(uint64 materialID)
    {
        iMaterial_oldPtr material = nullptr;

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
        uint64 result = iMaterial_old::INVALID_MATERIAL_ID;

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

        if (result == iMaterial_old::INVALID_MATERIAL_ID)
        {
            con_err("material with name:" << materialName << " does not exist");
        }

        return result;
    }

    iMaterial_oldPtr iMaterialResourceFactory::getMaterial(iaString materialName)
    {
        iMaterial_oldPtr material = 0;

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

    iMaterial_oldPtr iMaterialResourceFactory::getCurrentMaterial()
    {
        return _currentMaterial;
    }

} // namespace igor