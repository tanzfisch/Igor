// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iMaterialResourceFactory.h>

#include <iMaterialGroup.h>
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

        auto materialIter = _materials.begin();
        while (materialIter != _materials.end())
        {
            delete (*materialIter);
            ++materialIter;
        }

        _materials.clear();
        _materialMap.clear();
        _currentMaterial = 0;

        if(!_targetMaterials.empty())
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

        if(iter != _targetMaterials.end())
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

    bool compareGroup(iMaterialGroup* first, iMaterialGroup* second)
    {
        if (first->getMaterial()->getOrder() < second->getMaterial()->getOrder()) return true;
        else return false;
    }

    void iMaterialResourceFactory::updateGroups()
    {
        _mutexMaterial.lock();
        if (_dirtyMaterials)
        {
            _materials.sort(compareGroup);
            _dirtyMaterials = false;
        }
        _mutexMaterial.unlock();
    }

    list<iMaterialGroup*>* iMaterialResourceFactory::getMaterialGroups()
    {
        updateGroups();
        return &_materials;
    }

    void iMaterialResourceFactory::initDefaultMaterials()
    {
        // create the default material
        _defaultMaterial = createMaterial();
        getMaterial(_defaultMaterial)->setName("IgorDefault");
        getMaterial(_defaultMaterial)->addShaderSource("igor/default.vert", iShaderObjectType::Vertex);
        getMaterial(_defaultMaterial)->addShaderSource("igor/default_directional_light.frag", iShaderObjectType::Fragment);
        getMaterial(_defaultMaterial)->compileShader();
        getMaterial(_defaultMaterial)->setOrder(iMaterial::RENDER_ORDER_DEFAULT);

        // set material to start with
        setMaterial(_defaultMaterial);

        // create the color ID material
        _colorIDMaterial = createMaterial();
        getMaterial(_colorIDMaterial)->setName("IgorColorID");
        getMaterial(_colorIDMaterial)->addShaderSource("igor/default.vert", iShaderObjectType::Vertex);
        getMaterial(_colorIDMaterial)->addShaderSource("igor/solidColor.frag", iShaderObjectType::Fragment);
        getMaterial(_colorIDMaterial)->compileShader();
        getMaterial(_colorIDMaterial)->setOrder(iMaterial::RENDER_ORDER_DEFAULT);
    }

    uint64 iMaterialResourceFactory::createMaterial(iaString name)
    {
        iMaterialGroup* materialGroup = new iMaterialGroup();

        _materials.push_back(materialGroup);
        _materialMap[materialGroup->getID()] = materialGroup;

        _dirtyMaterials = true;

        if (name != L"")
        {
            materialGroup->getMaterial()->setName(name);
        }

        return materialGroup->getID();
    }

    void iMaterialResourceFactory::destroyMaterial(uint64 materialID)
    {
        _mutexMaterial.lock();

        auto iter = _materialMap.find(materialID);
        if (iter != _materialMap.end())
        {
            _materialMap.erase(iter);

            auto materialIter = _materials.begin();
            while (materialIter != _materials.end())
            {
                if ((*materialIter)->getID() == materialID)
                {
                    delete (*materialIter);
                    _materials.erase(materialIter);
                    break;
                }

                ++materialIter;
            }
        }
        else
        {
            con_err("material id: " << materialID << " does not exist");
        }

        _mutexMaterial.unlock();
    }

    void iMaterialResourceFactory::setMaterial(uint64 materialID)
    {
        iMaterial* material = 0;

        _mutexMaterial.lock();
        auto iter = _materialMap.find(materialID);
        if (iter != _materialMap.end())
        {
            material = (*iter).second->getMaterial();
        }
        _mutexMaterial.unlock();

        if (material)
        {
            con_assert_sticky(iRenderer::getInstance().isReady(), "renderer not ready");
            iRenderer::getInstance().setMaterial(material);
        }
    }

    iMaterial* iMaterialResourceFactory::getDefaultMaterial()
    {
        return getMaterial(_defaultMaterial);
    }

    iMaterial* iMaterialResourceFactory::getMaterial(uint64 materialID)
    {
        iMaterial* material = nullptr;

        _mutexMaterial.lock();
        auto iter = _materialMap.find(materialID);
        if (iter != _materialMap.end())
        {
            material = (*iter).second->getMaterial();
        }
        _mutexMaterial.unlock();

        if (!material)
        {
            con_err("material id: " << materialID << " does not exist");
        }

        return material;
    }

    iMaterialGroup* iMaterialResourceFactory::getMaterialGroup(uint64 materialID)
    {
        iMaterialGroup* materialGroup = nullptr;

        _mutexMaterial.lock();
        auto iter = _materialMap.find(materialID);
        if (iter != _materialMap.end())
        {
            materialGroup = (*iter).second;
        }
        _mutexMaterial.unlock();

        return materialGroup;
    }

    uint64 iMaterialResourceFactory::getMaterialID(iaString materialName)
    {
		uint64 result = iMaterial::INVALID_MATERIAL_ID;

        _mutexMaterial.lock();
        auto materialIter = _materials.begin();
        while (materialIter != _materials.end())
        {
            if ((*materialIter)->getMaterial()->getName() == materialName)
            {
                result = (*materialIter)->getID();
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

    iMaterial* iMaterialResourceFactory::getMaterial(iaString materialName)
    {
        iMaterial* material = 0;

        _mutexMaterial.lock();
        auto materialIter = _materials.begin();
        while (materialIter != _materials.end())
        {
            if ((*materialIter)->getMaterial()->getName() == materialName)
            {
                material = (*materialIter)->getMaterial();
                break;
            }

            ++materialIter;
        }

        _mutexMaterial.unlock();

        if (!material)
        {
            con_err("material with name:" << materialName << " does not exist");
        }

        return material;
    }

    iMaterial* iMaterialResourceFactory::getCurrentMaterial()
    {
        return _currentMaterial;
    }

}