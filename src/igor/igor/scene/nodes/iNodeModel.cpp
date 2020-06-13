// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/nodes/iNodeModel.h>

#include <igor/scene/nodes/iNode.h>
#include <igor/resources/model/iModelResourceFactory.h>
#include <igor/graphics/iRenderer.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodeMesh.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/scene/iScene.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/resources/mesh/iMeshBuffers.h>

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

	void iNodeModel::setModel(const iaString &modelFileName, iResourceCacheMode cacheMode, iModelDataInputParameter *parameters)
	{
		_filename = modelFileName;
		_parameters = parameters;
		_cacheMode = cacheMode;
		setDataDirty();
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

	bool iNodeModel::checkForBuffers(iNodePtr node)
	{
		if (node->getType() == iNodeType::iNodeMesh)
		{
			iNodeMesh *meshNode = static_cast<iNodeMesh *>(node);
			if (meshNode->getMeshBuffers() == nullptr ||
				!meshNode->getMeshBuffers()->isReady())
			{
				return false;
			}
		}

		for (auto child : node->getChildren())
		{
			if (!checkForBuffers(child))
			{
				return false;
			}
		}

		return true;
	}

	bool iNodeModel::checkForBuffers()
	{
		for (auto child : _children)
		{
			if (!checkForBuffers(child))
			{
				return false;
			}
		}

		return true;
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
} // namespace igor