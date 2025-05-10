// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iMeshReferenceComponent.h>
#include <igor/resources/iResourceManager.h>

namespace igor
{
    iMeshReferenceComponent::iMeshReferenceComponent()
    {
        
    }

    iEntityComponent *iMeshReferenceComponent::createInstance()
    {
        return new iMeshReferenceComponent();
    }

    const iaString& iMeshReferenceComponent::getTypeName()
    {
        static const iaString name("igor_mesh_reference_component");
        return name;
    }    

    iMeshReferenceComponent::iMeshReferenceComponent(iModelPtr model, const std::vector<iaString> &meshPaths)
        : _model(model), _meshPaths(meshPaths)
    {
    }

    iEntityComponentPtr iMeshReferenceComponent::getCopy()
    {
        iMeshReferenceComponent *component = new iMeshReferenceComponent();
        component->_model = _model;
        component->_meshPaths = _meshPaths;
        return component;
    }        

    iModelPtr iMeshReferenceComponent::getModel() const
    {
        return _model;
    }

    void iMeshReferenceComponent::setModel(iModelPtr model)
    {
        _model = model;
    }

    void iMeshReferenceComponent::setMeshPaths(const std::vector<iaString> &meshPaths)
    {
        _meshPaths = meshPaths;
    }

    const std::vector<iaString> &iMeshReferenceComponent::getMeshPaths() const
    {
        return _meshPaths;
    }

    std::vector<iaString> iMeshReferenceComponent::getInfo() const
    {
        std::vector<iaString> result = iEntityComponent::getInfo();

        result.push_back(iaString("Mdl: ") + _model->getID().toString());

        return result;
    }     
}