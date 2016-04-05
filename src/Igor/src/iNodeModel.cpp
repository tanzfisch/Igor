// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeModel.h>

#include <iModel.h>
#include <iNode.h>
#include <iModelResourceFactory.h>
#include <iRenderer.h>
#include <iNodeTransform.h>
#include <iNodeMesh.h>
#include <iNodeFactory.h>
#include <iScene.h>
#include <iMaterialResourceFactory.h>

#include <iaConsole.h>
using namespace IgorAux;

#include <vector>
using namespace std;

namespace Igor
{

    iNodeModel::iNodeModel()
        : iNode()
	{
		setName(L"iNodeModel");
		_nodeType = iNodeType::iNodeModel;
	}

    iNodeModel::iNodeModel(iNodeModel* node)
        : iNode()
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeType = node->_nodeType;
        _nodeKind = node->_nodeKind;

        setModel(node->getModelName());
    }

    iNodeModel::~iNodeModel()
    {
        if (_parameters != nullptr)
        {
            // if arrived here it was never given to iModel so we need to delete it now
            delete _parameters;
            _parameters = nullptr;
        }

        _model = nullptr;
    }

    bool iNodeModel::isLoaded()
    {
        return _initialized;
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

    void iNodeModel::setModel(const iaString& modelFileName, iModelDataInputParameter* parameters)
	{
        _filename = modelFileName;
        _parameters = parameters;
        setDataDirty();
	}

    void iNodeModel::onUpdateTransform(iaMatrixf& matrix)
    {
        if (!_initialized &&
            _model == nullptr)
        {
            _model = iModelResourceFactory::getInstance().requestModelData(_filename, _parameters);
            _parameters = nullptr; // passing ownership to iModel
        }
    }

	bool iNodeModel::onUpdateData()
	{
		if (!_initialized &&
            _model != nullptr &&
			_model->getState() == iModelState::Loaded)
		{
            insertNode(_model->getDataCopy());
			_initialized = true;
            return true;
		}

        return false;
	}
}