// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlTransformation.h"

#include "../../MicaDefines.h"

UserControlTransformation::UserControlTransformation(iNodeID nodeID, const iWidgetPtr parent)
	: UserControlNode(nodeID, parent)
{
}

void UserControlTransformation::init()
{
	UserControlNode::init();

	iWidgetGroupBoxPtr group = new iWidgetGroupBox(_layout);
	group->setHorizontalAlignment(iHorizontalAlignment::Stretch);
	group->setText("Matrix");

	_grid = new iWidgetGridLayout(group);
	_grid->setHorizontalAlignment(iHorizontalAlignment::Right);
	_grid->setVerticalAlignment(iVerticalAlignment::Top);
	_grid->appendColumns(3);
	_grid->appendRows(3);

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

void UserControlTransformation::update()
{
	UserControlNode::update();

	// check if new id is a valid node
	iNodePtr node = iNodeManager::getInstance().getNode(getNodeID());
	if (node == nullptr ||
		node->getType() != iNodeType::iNodeTransform)
	{
		return;
	}

	node->getTransformationChangeEvent().add(iTransformationChangeDelegate(this, &UserControlTransformation::onTransformationChanged));
	onTransformationChanged(node);
}

UserControlTransformation::~UserControlTransformation()
{
	iNode *node = iNodeManager::getInstance().getNode(_nodeId);
	if (node != nullptr)
	{
		node->getTransformationChangeEvent().remove(iTransformationChangeDelegate(this, &UserControlTransformation::onTransformationChanged));
	}
}

void UserControlTransformation::onTransformationChanged(iNode *node)
{
	con_assert(node->getID() == _nodeId && node->getType() == iNodeType::iNodeTransform, "internal error");

	iNodeTransformPtr transformNode = static_cast<iNodeTransformPtr>(node);
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
	textEdit->setMinWidth(MICA_REGULARBUTTON_SIZE);
	textEdit->setMaxTextLength(11);
	textEdit->setHorizontalAlignment(iHorizontalAlignment::Left);
	textEdit->setVerticalAlignment(iVerticalAlignment::Top);
	textEdit->setEnabled(false);

	return textEdit;
}
