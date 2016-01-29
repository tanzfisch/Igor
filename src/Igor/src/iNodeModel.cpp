// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
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

    iNodeModelQueue iNodeModel::modelNodeQueue;

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
        _model = nullptr;
    }

    iaString iNodeModel::getModelName() const
    {
        return _filename;
    }

    void iNodeModel::processModelNodes()
    {
        modelNodeQueue.process();
    }

    void iNodeModel::setModel(const iaString& modelFileName, iModelDataInputParameter* parameters)
	{
        _filename = modelFileName;
        _parameters = parameters;
	}

    void iNodeModel::onUpdateTransform(iaMatrixf& matrix)
    {
        if (!_initialized &&
            _model == nullptr)
        {
            _model = iModelResourceFactory::getInstance().requestModelData(_filename, _parameters);
            _parameters = nullptr;
            modelNodeQueue.addModelNode(this);
        }
    }

	bool iNodeModel::updateModelData()
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