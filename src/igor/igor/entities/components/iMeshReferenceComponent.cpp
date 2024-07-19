// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iMeshReferenceComponent.h>
#include <igor/resources/iResourceManager.h>

namespace igor
{
    iMeshReferenceComponent::iMeshReferenceComponent()
    {
        
    }

    iMeshReferenceComponent::iMeshReferenceComponent(iModelPtr model, const iaString &meshPath)
        : _model(model), _meshPath(meshPath)
    {
    }

    iEntityComponentPtr iMeshReferenceComponent::getCopy()
    {
        iMeshReferenceComponent *component = new iMeshReferenceComponent();
        component->_model = _model;
        component->_meshPath = _meshPath;
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

    void iMeshReferenceComponent::setMeshPath(const iaString &meshPath)
    {
        _meshPath = meshPath;
    }

    const iaString &iMeshReferenceComponent::getMeshPath() const
    {
        return _meshPath;
    }
}