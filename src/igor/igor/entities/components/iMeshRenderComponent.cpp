// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iMeshRenderComponent.h>

namespace igor
{
    iMeshRenderComponent::iMeshRenderComponent(iMeshPtr mesh, iMaterialPtr material, const iaString &name)
        : iEntityComponent(name), _mesh(mesh), _material(material)
    {
    }

    bool iMeshRenderComponent::onLoad(iEntityPtr entity)
    {
        return _mesh != nullptr && _material != nullptr;
    }

    iMeshPtr iMeshRenderComponent::getMesh() const
    {
        return _mesh;
    }

    iMaterialPtr iMeshRenderComponent::getMaterial() const
    {
        return _material;
    }
}