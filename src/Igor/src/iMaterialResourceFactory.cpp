// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iMaterialResourceFactory.h>

#include <iMaterialGroup.h>
#include <iMaterial.h>
#include <iShader.h>
#include <iRenderer.h>
#include <iTargetMaterial.h>

namespace Igor
{

	iMaterialResourceFactory::iMaterialResourceFactory()
	{
        initDefaultMaterial();
	}

	iMaterialResourceFactory::~iMaterialResourceFactory()
	{
        destroyMaterial(_defaultID);

        if (!_materials.empty())
        {
            con_endl(static_cast<int>(_materials.size()) << " materials left. will clean up for you");
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
        _targetMaterials.push_back(result);

        return result;
    }

    void iMaterialResourceFactory::destroyTargetMaterial(iTargetMaterial* targetMaterial)
    {
        auto iter = find(_targetMaterials.begin(), _targetMaterials.end(), targetMaterial);

        if(iter != _targetMaterials.end())
        {
            delete (*iter);
            _targetMaterials.erase(iter);
        }
    }

    int32 iMaterialResourceFactory::getDefaultMaterialID() const
    {
        return _defaultID;
    }

    bool compareGroup(iMaterialGroup* first, iMaterialGroup* second)
    {
        if (first->getOrder() < second->getOrder()) return true;
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

    /*! \todo move all except the default shader to config file
    */
    void iMaterialResourceFactory::initDefaultMaterial()
    {
        _defaultID = createMaterial();
        getMaterial(_defaultID)->setName("IgorDefault");
        getMaterial(_defaultID)->addShaderSource("default.vert", iShaderObjectType::Vertex);
        getMaterial(_defaultID)->addShaderSource("default_directional_light.frag", iShaderObjectType::Fragment);
        getMaterial(_defaultID)->compileShader();
        getMaterial(_defaultID)->getRenderStateSet().setRenderState(iRenderState::Lighting, iRenderStateValue::Off);
        getMaterial(_defaultID)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::Off);
        getMaterialGroup(_defaultID)->setOrder(100);

        // set material to start with
        setMaterial(_defaultID);
    }

    int32 iMaterialResourceFactory::createMaterial(iaString name)
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

    void iMaterialResourceFactory::destroyMaterial(int32 materialID)
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

    void iMaterialResourceFactory::setMaterial(int32 materialID)
    {
        iMaterial* material = 0;

        _mutexMaterial.lock();
        auto iter = _materialMap.find(materialID);
        if (iter != _materialMap.end())
        {
            material = (*iter).second->getMaterial();
        }
        _mutexMaterial.unlock();

        if (material &&
            iRenderer::getInstance().isReady())
        {
            iRenderer::getInstance().setMaterial(material);
        }
    }

    iMaterial* iMaterialResourceFactory::getDefaultMaterial()
    {
        return getMaterial(_defaultID);
    }

    iMaterial* iMaterialResourceFactory::getMaterial(int32 materialID)
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

    iMaterialGroup* iMaterialResourceFactory::getMaterialGroup(int32 materialID)
    {
        iMaterialGroup* materialGroup = nullptr;

        _mutexMaterial.lock();
        auto iter = _materialMap.find(materialID);
        if (iter != _materialMap.end())
        {
            materialGroup = (*iter).second;
        }
        _mutexMaterial.unlock();

        if (!materialGroup)
        {
            con_err("material id: " << materialID << " does not exist");
        }

        return materialGroup;
    }

    uint32 iMaterialResourceFactory::getMaterialID(iaString materialName)
    {
        uint32 result = 0;

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

        if (0 == result)
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