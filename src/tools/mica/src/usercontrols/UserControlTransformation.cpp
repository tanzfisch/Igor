// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlTransformation.h"

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/widgets/iWidgetGrid.h>
#include <igor/ui/widgets/iWidgetTextEdit.h>
#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetButton.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodeManager.h>
using namespace igor;

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

	iaVector3d scale;
	iaQuaterniond orientation;
	iaVector3d rotate;
	iaVector3d translate;
	iaVector3d shear;
	iaVector4d perspective;
	matrix.decompose(scale, orientation, translate, shear, perspective);

	orientation.getEuler(rotate);

	_translateText[0]->setText(iaString::toString(translate._x, 4));
	_translateText[1]->setText(iaString::toString(translate._y, 4));
	_translateText[2]->setText(iaString::toString(translate._z, 4));

	_rotateText[0]->setText(iaString::toString(rotate._x / M_PI * 180.0, 4));
	_rotateText[1]->setText(iaString::toString(rotate._y / M_PI * 180.0, 4));
	_rotateText[2]->setText(iaString::toString(rotate._z / M_PI * 180.0, 4));

	_scaleText[0]->setText(iaString::toString(scale._x, 4));
	_scaleText[1]->setText(iaString::toString(scale._y, 4));
	_scaleText[2]->setText(iaString::toString(scale._z, 4));

	_shearText[0]->setText(iaString::toString(shear._x, 4));
	_shearText[1]->setText(iaString::toString(shear._y, 4));
	_shearText[2]->setText(iaString::toString(shear._z, 4));
}

iWidgetTextEdit *UserControlTransformation::createTextEdit()
{
	iWidgetTextEdit *textEdit = new iWidgetTextEdit();
	textEdit->setText("");
	textEdit->setWidth(MICA_REGULARBUTTON_SIZE);
	textEdit->setMaxTextLength(11);
	textEdit->setHorizontalAlignment(iHorizontalAlignment::Left);
	textEdit->setVerticalAlignment(iVerticalAlignment::Top);
	textEdit->registerOnChangeEvent(iChangeDelegate(this, &UserControlTransformation::onChange));

	return textEdit;
}

void UserControlTransformation::initGUI()
{
	_grid = new iWidgetGrid();
	_grid->setHorizontalAlignment(iHorizontalAlignment::Right);
	_grid->setVerticalAlignment(iVerticalAlignment::Top);
	_grid->appendCollumns(3);
	_grid->appendRows(3);
	addWidget(_grid);

	for (int i = 0; i < 3; ++i)
	{
		iWidgetTextEdit *textEdit = createTextEdit();
		_translateText.push_back(textEdit);
		_grid->addWidget(textEdit, i + 1, 0);
	}

	for (int i = 0; i < 3; ++i)
	{
		iWidgetTextEdit *textEdit = createTextEdit();
		_scaleText.push_back(textEdit);
		_grid->addWidget(textEdit, i + 1, 1);
	}

	for (int i = 0; i < 3; ++i)
	{
		iWidgetTextEdit *textEdit = createTextEdit();
		_rotateText.push_back(textEdit);
		_grid->addWidget(textEdit, i + 1, 2);
	}

	for (int i = 0; i < 3; ++i)
	{
		iWidgetTextEdit *textEdit = createTextEdit();
		_shearText.push_back(textEdit);
		_grid->addWidget(textEdit, i + 1, 3);
	}

	iWidgetLabel *translateLabel = new iWidgetLabel();
	translateLabel->setHorizontalAlignment(iHorizontalAlignment::Left);
	translateLabel->setText("Translate");
	iWidgetLabel *scaleLabel = new iWidgetLabel();
	scaleLabel->setHorizontalAlignment(iHorizontalAlignment::Left);
	scaleLabel->setText("Scale");
	iWidgetLabel *rotateLabel = new iWidgetLabel();
	rotateLabel->setHorizontalAlignment(iHorizontalAlignment::Left);
	rotateLabel->setText("Rotate");
	iWidgetLabel *shearLabel = new iWidgetLabel();
	shearLabel->setHorizontalAlignment(iHorizontalAlignment::Left);
	shearLabel->setText("Shear");

	_grid->addWidget(translateLabel, 0, 0);
	_grid->addWidget(scaleLabel, 0, 1);
	_grid->addWidget(rotateLabel, 0, 2);
	_grid->addWidget(shearLabel, 0, 3);
}

void UserControlTransformation::onChange(const iWidgetPtr source)
{
	iNodeTransform *node = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_nodeId));

	if (node != nullptr)
	{
		iaMatrixd matrix;

		// translate
		matrix.translate(iaString::toFloat(_translateText[0]->getText()),
						 iaString::toFloat(_translateText[1]->getText()),
						 iaString::toFloat(_translateText[2]->getText()));

		// rotate order zyx
		matrix.rotate(iaString::toFloat(_rotateText[2]->getText()) / 180.0 * M_PI, iaAxis::Z);
		matrix.rotate(iaString::toFloat(_rotateText[1]->getText()) / 180.0 * M_PI, iaAxis::Y);
		matrix.rotate(iaString::toFloat(_rotateText[0]->getText()) / 180.0 * M_PI, iaAxis::X);

		// scale
		matrix.scale(iaString::toFloat(_scaleText[0]->getText()),
					 iaString::toFloat(_scaleText[1]->getText()),
					 iaString::toFloat(_scaleText[2]->getText()));

		// shear
		matrix.shear(iaString::toFloat(_shearText[0]->getText()),
					 iaString::toFloat(_shearText[1]->getText()),
					 iaString::toFloat(_shearText[2]->getText()));

		node->setMatrix(matrix);
	}
}
