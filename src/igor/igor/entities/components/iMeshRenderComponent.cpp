// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iMeshRenderComponent.h>
#include <igor/resources/iResourceManager.h>

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

    void iMeshRenderComponent::setMesh(iMeshPtr mesh)
    {
        con_assert(mesh != nullptr, "not allowed to reset the mesh");
        _mesh = mesh;
    }

    void iMeshRenderComponent::setMaterial(iMaterialPtr material)
    {
        con_assert(material != nullptr, "not allowed to reset the material");
        _material = material;
    }

    iMaterialPtr iMeshRenderComponent::getMaterial() const
    {
        return _material;
    }
}