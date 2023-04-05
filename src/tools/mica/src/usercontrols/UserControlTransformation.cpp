// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlTransformation.h"

#include "../MicaDefines.h"

UserControlTransformation::UserControlTransformation()
{
	initGUI();
}

UserControlTransformation::~UserControlTransformation()
{
	iNode *node = iNodeManager::getInstance().getNode(_nodeId);
	if (node != nullptr)
	{
		node->getTransformationChangeEvent().remove(iTransformationChangeDelegate(this, &UserControlTransformation::onTransformationChanged));
	}
}

void UserControlTransformation::setNode(uint32 id)
{
	if (_nodeId == id)
	{
		return;
	}

	// unregister from current node
	iNode *oldNode = iNodeManager::getInstance().getNode(_nodeId);
	if (oldNode != nullptr)
	{
		oldNode->getTransformationChangeEvent().remove(iTransformationChangeDelegate(this, &UserControlTransformation::onTransformationChanged));
	}

	// check if new id is a valid node
	iNode *node = iNodeManager::getInstance().getNode(id);
	if (node == nullptr)
	{
		return;
	}

	if (node->getType() != iNodeType::iNodeTransform)
	{
		return;
	}

	// register to new node id
	_nodeId = id;
	node->getTransformationChangeEvent().append(iTransformationChangeDelegate(this, &UserControlTransformation::onTransformationChanged));

	updateGUI(static_cast<iNodeTransform *>(node));
}

void UserControlTransformation::onTransformationChanged(iNode *node)
{
	if (node->getID() != _nodeId)
	{
		con_err("registered to change event of wrong node " << node->getID() << " should be " << _nodeId);
		return;
	}

	updateGUI(static_cast<iNodeTransform *>(node));
}

uint32 UserControlTransformation::getNode()
{
	return _nodeId;
}

void UserControlTransformation::updateGUI(iNodeTransform *transformNode)
{
	iaMatrixd matrix;
	transformNode->getMatrix(matrix);

	for (int i = 0; i < 16; ++i)
	{
		_matrixText[i]->setText(iaString::toString(matrix[i], 4));
	}
}

iWidgetLineTextEdit *UserControlTransformation::createTextEdit()
{
	iWidgetLineTextEdit *textEdit = new iWidgetLineTextEdit();
	textEdit->setText("");
	textEdit->setWidth(MICA_REGULARBUTTON_SIZE);
	textEdit->setMaxTextLength(11);
	textEdit->setHorizontalAlignment(iHorizontalAlignment::Left);
	textEdit->setVerticalAlignment(iVerticalAlignment::Top);
	textEdit->setEnabled(false);

	return textEdit;
}

void UserControlTransformation::initGUI()
{
	_grid = new iWidgetGrid();
	_grid->setHorizontalAlignment(iHorizontalAlignment::Right);
	_grid->setVerticalAlignment(iVerticalAlignment::Top);
	_grid->appendColumns(3);
	_grid->appendRows(3);
	addWidget(_grid);

	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			iWidgetLineTextEdit *textEdit = createTextEdit();
			_matrixText.push_back(textEdit);
			_grid->addWidget(textEdit, row, col);
		}
	}
}
