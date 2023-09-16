// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/nodes/iNodeModel.h>

#include <igor/scene/nodes/iNode.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodeMesh.h>
#include <igor/scene/nodes/iNodeRender.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/scene/iScene.h>
#include <igor/resources/iResourceManager.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <vector>

namespace igor
{

    iNodeModel::iNodeModel()
        : iNode()
    {
        setName(L"iNodeModel");
        _nodeType = iNodeType::iNodeModel;
    }

    iNodeModel::iNodeModel(iNodeModel *node)
        : iNode()
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeType = node->_nodeType;
        _nodeKind = node->_nodeKind;

        setModel(node->_model);
    }

    iNodeModel::~iNodeModel()
    {
        setScene(nullptr);

        _model = nullptr;
    }

    void iNodeModel::registerModelReadyDelegate(iModelReadyDelegate delegate)
    {
        _modelReadyEvent.add(delegate);
    }

    void iNodeModel::unregisterModelReadyDelegate(iModelReadyDelegate delegate)
    {
        _modelReadyEvent.remove(delegate);
    }

    bool iNodeModel::isValid() const
    {
        return _model != nullptr ? _model->isValid() : false;
    }

    bool iNodeModel::isLoaded() const 
    {
        return _model != nullptr ? _model->isProcessed() : false;
    }

    void iNodeModel::onPreSetScene()
    {
        // nothing to do
    }

    void iNodeModel::onPostSetScene()
    {
        if (getScene() &&
            isDataDirty())
        {
            getScene()->addToDataUpdateQueue(this);
        }
    }

    void iNodeModel::setMaterial(const iMaterialPtr &material)
    {
        _material = material;

        if (!isLoaded())
        {
            return;
        }

        setMaterial(this, material);
    }

    void iNodeModel::setMaterial(iNodePtr node, const iMaterialPtr &material)
    {
        if (node->getType() == iNodeType::iNodeMesh)
        {
            iNodeMeshPtr meshNode = static_cast<iNodeMeshPtr>(node);
            meshNode->setMaterial(material);
        }

        std::vector<iNodePtr> children;
        node->getAllChildren(children);
        for (auto &child : children)
        {
            setMaterial(child, material);
        }
    }

    void iNodeModel::setModel(iModelPtr model)
    {
        _model = model;

        if(_model == nullptr)
        {
            return;
        }

        if(_model->isValid())
        {
            onUpdateData();
        }
        else if(!_model->isProcessed())
        {
            setDataDirty();
        }
    }

    void iNodeModel::setModel(const iaString &name)
    {
        if (iResourceManager::getInstance().getLoadMode() == iResourceManagerLoadMode::Synchronized)
        {
            _model = iResourceManager::getInstance().loadResource<iModel>(name);
            onUpdateData();
        }
        else
        {
            _model = iResourceManager::getInstance().requestResource<iModel>(name);
            setDataDirty();
        }
    }

    void iNodeModel::onUpdateTransform(iaMatrixd &matrix)
    {
        // nothing to do
    }

    bool iNodeModel::onUpdateData()
    {
        if (_model == nullptr)
        {
            return false;
        }

        if (_model->isValid())
        {
            insertNode(_model->getNodeCopy());

            if (_material != nullptr)
            {
                setMaterial(_material);
            }

            _modelReadyEvent(getID());
        }

        return _model->isValid();
    }

    iModelPtr iNodeModel::getModel() const
    {
        return _model;
    }

    const iaString iNodeModel::getModelName() const
    {
        return _model != nullptr ? _model->getAlias() : "";
    }

    void iNodeModel::getInfo(std::vector<iaString> &info) const
    {
        iNode::getInfo(info);
        iaString customInfo(L"model:");

        if (_model != nullptr)
        {
            customInfo += getModelName();
            customInfo += L" loaded:";
            if (isValid())
            {
                customInfo += L"true ready:";
                if (isLoaded())
                {
                    customInfo += L"true";
                    customInfo += L" nodeID:";
                    customInfo += iaString::toString(_model->getNode()->getID());
                }
                else
                {
                    customInfo += L"false";
                }
            }
            else
            {
                customInfo += L"false";
            }
        }
        else
        {
            customInfo += L"none";
        }

        info.push_back(customInfo);
    }
} // namespace igor