// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iMeshRenderComponent.h>

#include <igor/entities/components/iMeshReferenceComponent.h>
#include <igor/scene/nodes/iNodeMesh.h>
#include <igor/resources/iResourceManager.h>

namespace igor
{

    iMeshRenderComponent::iMeshRenderComponent()
    {
        
    }

    iMeshRenderComponent::iMeshRenderComponent(iMeshPtr mesh, iMaterialPtr material)
        : _mesh(mesh), _material(material)
    {
    }

    iEntityComponentPtr iMeshRenderComponent::getCopy()
    {
        iMeshRenderComponent *component = new iMeshRenderComponent();
        component->_mesh = _mesh;
        component->_material = _material;
        return component;
    }          

    void iMeshRenderComponent::onUnLoad(iEntityPtr entity)
    {
        _mesh = nullptr;
        _material = nullptr;
    }

    bool iMeshRenderComponent::onLoad(iEntityPtr entity, bool &asyncLoad)
    {
        asyncLoad = false; // stop trying

        if(_mesh != nullptr && _material != nullptr)
        {
            return true;
        }

        auto reference = entity->getComponent<iMeshReferenceComponent>();
        if(reference == nullptr ||
            reference->getModel() == nullptr)
        {
            con_err("no model reference");
            return false;
        }

        if(!reference->getModel()->isValid())
        {
            asyncLoad = true; // keep trying
            return false;
        }

        auto meshPath = reference->getMeshPath();
        std::vector<iaString> tokens;
        meshPath.split('/', tokens);

        if(tokens.empty())
        {
            con_err("invalid mesh path \"" << meshPath << "\"");
            return false;
        }

        auto model = reference->getModel();
        iNodePtr node = model->getNode();
        if(node->getName() != *tokens.begin())
        {
            return false;
        }

        for(int i=1;i<tokens.size();++i)
        {
            node = node->getChild(tokens[i]);

            if(node == nullptr)
            {
                return false;
            }
        }

        if(node->getType() != iNodeType::iNodeMesh)
        {
            return false;
        }

        iNodeMeshPtr meshNode = static_cast<iNodeMeshPtr>(node);
        _mesh = meshNode->getMesh();
        _material = meshNode->getMaterial();

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