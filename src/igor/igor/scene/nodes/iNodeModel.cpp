// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/nodes/iNodeModel.h>

#include <igor/scene/nodes/iNode.h>
#include <igor/resources/model/iModelResourceFactory.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodeMesh.h>
#include <igor/scene/nodes/iNodeRender.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/scene/iScene.h>
#include <igor/resources/material/iMaterialResourceFactory.h>

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

        setModel(node->getModelName(), node->_cacheMode);
    }

    iNodeModel::~iNodeModel()
    {
        setScene(nullptr);

        if (_parameters != nullptr)
        {
            // if arrived here it was never given to iModel so we need to delete it now
            delete _parameters;
            _parameters = nullptr;
        }

        _model = nullptr;
    }

    void iNodeModel::registerModelReadyDelegate(iModelReadyDelegate delegate)
    {
        _modelReadyEvent.append(delegate);
    }

    void iNodeModel::unregisterModelReadyDelegate(iModelReadyDelegate delegate)
    {
        _modelReadyEvent.remove(delegate);
    }

    bool iNodeModel::isValid()
    {
        return _ready;
    }

    bool iNodeModel::isLoaded()
    {
        return _loaded;
    }

    iaString iNodeModel::getModelName() const
    {
        return _filename;
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

    void iNodeModel::setMaterial(const iMaterialPtr& material)
    {
        if (!isLoaded())
        {
            con_err("can't set material on unloaded model");
            return;
        }

        setMaterial(this, material);
    }

    void iNodeModel::setMaterial(iNodePtr node, const iMaterialPtr& material)
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

    void iNodeModel::setModel(const iaString &modelFileName, iResourceCacheMode cacheMode, iModelDataInputParameter *parameters, bool loadSynchronously)
    {
        _filename = modelFileName;
        _cacheMode = cacheMode;
        _parameters = parameters;

        if (loadSynchronously)
        {
            _model = iModelResourceFactory::getInstance().loadModelData(_filename, _cacheMode, _parameters);
            _parameters = nullptr; // passing ownership to iModel

            onUpdateData();
        }
        else
        {
            setDataDirty();
        }
    }

    void iNodeModel::onUpdateTransform(iaMatrixd &matrix)
    {
        if (!_loaded &&
            _model == nullptr)
        {
            _model = iModelResourceFactory::getInstance().requestModelData(_filename, _cacheMode, _parameters);
            _parameters = nullptr; // passing ownership to iModel
        }
    }

    bool iNodeModel::onUpdateData()
    {
        if (!_loaded &&
            _model != nullptr)
        {
            if (_model->getState() == iModelState::Loaded)
            {
                insertNode(_model->getNodeCopy());
                _loaded = true;
                _ready = true;
                _modelReadyEvent(getID());
            }
            else if (_model->getState() == iModelState::LoadFailed)
            {
                _loaded = true;
                _ready = false;
            }
        }

        return _loaded;
    }

    void iNodeModel::getInfo(std::vector<iaString> &info) const
    {
        iNode::getInfo(info);
        iaString customInfo(L"model:");

        if (_model != nullptr)
        {
            customInfo += _filename;
            customInfo += L" loaded:";
            if (_loaded)
            {
                customInfo += L"true ready:";
                if (_ready)
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