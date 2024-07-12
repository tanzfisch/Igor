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

    iMaterialPtr iMeshRenderComponent::getMaterial() const
    {
        return _material;
    }

    void to_json(json &j, const iMeshRenderComponent &component)
    {
        // TODO by reference or do we want to embed the data? j["mesh"] = component.getMesh()->
        j["material"] = component.getMaterial()->getID();
    }

    void from_json(const json &j, iMeshRenderComponent &component)
    {
        auto materialID = j["material"].get<iaUUID>();
        component._material = iResourceManager::getInstance().requestResource<iMaterial>(materialID);

        // TODO mesh
    }
}