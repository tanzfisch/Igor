// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iMeshRenderComponent.h>

#include <igor/entities/components/iMeshReferenceComponent.h>
#include <igor/resources/iResourceManager.h>

#include <stack>

namespace igor
{

    iMeshRenderComponent::iMeshRenderComponent()
    {
    }

    iEntityComponent *iMeshRenderComponent::createInstance()
    {
        return new iMeshRenderComponent();
    }

    iEntityComponentPtr iMeshRenderComponent::getCopy()
    {
        iMeshRenderComponent *component = new iMeshRenderComponent();
        component->_meshReferences = _meshReferences;
        return component;
    }

    void iMeshRenderComponent::onUnLoad(iEntityPtr entity)
    {
        _meshReferences.clear();
    }

    void iMeshRenderComponent::findAndAddMeshs(iNodePtr node)
    {
        if (node->getType() == iNodeType::iNodeMesh)
        {
            iNodeMeshPtr meshNode = static_cast<iNodeMeshPtr>(node);
            _meshReferences.emplace_back(meshNode->getMesh(), meshNode->getMaterial(), meshNode->getWorldMatrix());
        }

        const auto &children = node->getChildren();
        for (const auto child : children)
        {
            findAndAddMeshs(child);
        }
    }

    bool iMeshRenderComponent::onLoad(iEntityPtr entity, bool &asyncLoad)
    {
        asyncLoad = false; // stop trying

        if (!_meshReferences.empty())
        {
            return true;
        }

        auto reference = entity->getComponent<iMeshReferenceComponent>();
        if (reference == nullptr ||
            reference->getModel() == nullptr)
        {
            con_err("no model reference");
            return false;
        }

        if (!reference->getModel()->isValid())
        {
            asyncLoad = true; // keep trying
            return false;
        }

        auto model = reference->getModel();
        iNodePtr node = model->getNode();

        const auto &meshPaths = reference->getMeshPaths();
        if (meshPaths.empty())
        {
            findAndAddMeshs(node);
        }
        else
        {
            for (const auto &meshPath : meshPaths)
            {
                std::vector<iaString> tokens;
                meshPath.split('/', tokens);

                if (tokens.empty())
                {
                    con_err("invalid mesh path \"" << meshPath << "\"");
                    continue;
                }

                if (node->getName() != *tokens.begin())
                {
                    continue;
                }

                bool fail = false;
                for (int i = 1; i < tokens.size(); ++i)
                {
                    node = node->getChild(tokens[i]);

                    if (node == nullptr)
                    {
                        fail = true;
                    }
                }

                if (fail ||
                    node->getType() != iNodeType::iNodeMesh)
                {
                    continue;
                }

                iNodeMeshPtr meshNode = static_cast<iNodeMeshPtr>(node);
                _meshReferences.emplace_back(meshNode->getMesh(), meshNode->getMaterial(), meshNode->getWorldMatrix());
            }
        }

        return !_meshReferences.empty();
    }

    const std::vector<iMeshReference> &iMeshRenderComponent::getMeshReferences() const
    {
        return _meshReferences;
    }
}