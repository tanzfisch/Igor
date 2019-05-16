// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlTransformation.h"

#include <iWidgetManager.h>
#include <iWidgetGrid.h>
#include <iWidgetTextEdit.h>
#include <iWidgetLabel.h>
#include <iWidgetButton.h>
#include <iNodeTransform.h>
#include <iNodeFactory.h>
using namespace Igor;

#include "ModelViewerDefines.h"

UserControlTransformation::UserControlTransformation()
{
	initGUI();
}

UserControlTransformation::~UserControlTransformation()
{
	deinitGUI();
}

void UserControlTransformation::setNode(uint32 id)
{
	_nodeId = id;
	updateGUI();
}

uint32 UserControlTransformation::getNode()
{
	return _nodeId;
}

void UserControlTransformation::updateGUI()
{
	iNodeTransform* node = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_nodeId));

	if (node != nullptr)
	{
		iaMatrixd matrix;
		node->getMatrix(matrix);

		iaVector3d scale;
		iaQuaterniond orientation;
		iaVector3d rotate;
		iaVector3d translate;
		iaVector3d shear;
		iaVector4d perspective;
		matrix.decompose(scale, orientation, translate, shear, perspective);

		orientation.getEuler(rotate);

		_translateText[0]->setText(iaString::ftoa(translate._x, 4));
		_translateText[1]->setText(iaString::ftoa(translate._y, 4));
		_translateText[2]->setText(iaString::ftoa(translate._z, 4));

		_rotateText[0]->setText(iaString::ftoa(rotate._x / M_PI * 180.0, 4));
		_rotateText[1]->setText(iaString::ftoa(rotate._y / M_PI * 180.0, 4));
		_rotateText[2]->setText(iaString::ftoa(rotate._z / M_PI * 180.0, 4));

		_scaleText[0]->setText(iaString::ftoa(scale._x, 4));
		_scaleText[1]->setText(iaString::ftoa(scale._y, 4));
		_scaleText[2]->setText(iaString::ftoa(scale._z, 4));

		_shearText[0]->setText(iaString::ftoa(shear._x, 4));
		_shearText[1]->setText(iaString::ftoa(shear._y, 4));
		_shearText[2]->setText(iaString::ftoa(shear._z, 4));
	}
}

void UserControlTransformation::deinitGUI()
{
	auto iter = _allWidgets.begin();
	while (iter != _allWidgets.end())
	{
		iWidgetManager::getInstance().destroyWidget((*iter));
		iter++;
	}

	_allWidgets.clear();
	_translateText.clear();
	_scaleText.clear();
	_rotateText.clear();
	_shearText.clear();
}

iWidgetTextEdit* UserControlTransformation::createTextEdit()
{
	iWidgetTextEdit* textEdit = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget("TextEdit"));
	_allWidgets.push_back(textEdit);
	textEdit->setText("");
	textEdit->setWidth(MV_REGULARBUTTON_SIZE);
	textEdit->setMaxTextLength(11);
	textEdit->setHorizontalAlignment(iHorizontalAlignment::Left);
	textEdit->setVerticalAlignment(iVerticalAlignment::Top);
	textEdit->registerOnChangeEvent(iChangeDelegate(this, &UserControlTransformation::onChange));

	return textEdit;
}

void UserControlTransformation::initGUI()
{
	_grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
	_allWidgets.push_back(_grid);
	_grid->setHorizontalAlignment(iHorizontalAlignment::Right);
	_grid->setVerticalAlignment(iVerticalAlignment::Top);
	_grid->appendCollumns(3);
	_grid->appendRows(3);

	for (int i = 0; i < 3; ++i)
	{
		iWidgetTextEdit* textEdit = createTextEdit();
		_translateText.push_back(textEdit);
		_grid->addWidget(textEdit, i + 1, 0);
	}

	for (int i = 0; i < 3; ++i)
	{
		iWidgetTextEdit* textEdit = createTextEdit();
		_scaleText.push_back(textEdit);
		_grid->addWidget(textEdit, i + 1, 1);
	}

	for (int i = 0; i < 3; ++i)
	{
		iWidgetTextEdit* textEdit = createTextEdit();
		_rotateText.push_back(textEdit);
		_grid->addWidget(textEdit, i + 1, 2);
	}

	for (int i = 0; i < 3; ++i)
	{
		iWidgetTextEdit* textEdit = createTextEdit();
		_shearText.push_back(textEdit);
		_grid->addWidget(textEdit, i + 1, 3);
	}

	iWidgetLabel* translateLabel = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
	translateLabel->setText("Translate");
	iWidgetLabel* scaleLabel = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
	scaleLabel->setText("Scale");
	iWidgetLabel* rotateLabel = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
	rotateLabel->setText("Rotate");
	iWidgetLabel* shearLabel = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
	shearLabel->setText("Shear");

	_grid->addWidget(translateLabel, 0, 0);
	_grid->addWidget(scaleLabel, 0, 1);
	_grid->addWidget(rotateLabel, 0, 2);
	_grid->addWidget(shearLabel, 0, 3);
}

void UserControlTransformation::onChange(iWidget* source)
{
	iNodeTransform* node = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_nodeId));

	if (node != nullptr)
	{
		iaMatrixd matrix;

		// translate
		matrix.translate(iaString::atof(_translateText[0]->getText()),
			iaString::atof(_translateText[1]->getText()),
			iaString::atof(_translateText[2]->getText()));

		// rotate order xyz
		matrix.rotate(iaString::atof(_rotateText[2]->getText()) / 180.0 * M_PI, iaAxis::Z);
		matrix.rotate(iaString::atof(_rotateText[1]->getText()) / 180.0 * M_PI, iaAxis::Y);
		matrix.rotate(iaString::atof(_rotateText[0]->getText()) / 180.0 * M_PI, iaAxis::X);
			
		// scale
		matrix.scale(iaString::atof(_scaleText[0]->getText()),
			iaString::atof(_scaleText[1]->getText()),
			iaString::atof(_scaleText[2]->getText()));

		// shear
		matrix.shear(iaString::atof(_shearText[0]->getText()),
			iaString::atof(_shearText[1]->getText()),
			iaString::atof(_shearText[2]->getText()));

		node->setMatrix(matrix);
	}
}

iWidget* UserControlTransformation::getWidget()
{
	return _grid;
}