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

        for (int i = 0; i < 16; ++i)
        {
            _matrixText[i]->setText(iaString::ftoa(matrix[i], 2));
        }

        for (int i = 0; i < 3; ++i)
        {
            _translateText[i]->setText("0.0");
        }

        for (int i = 0; i < 3; ++i)
        {
            _rotateText[i]->setText("0.0");
        }

        for (int i = 0; i < 3; ++i)
        {
            _scaleText[i]->setText("1.0");
        }
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

    _matrixText.clear();
}

void UserControlTransformation::initGUI()
{
    _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _allWidgets.push_back(_grid);
    _grid->setHorizontalAlignment(iHorizontalAlignment::Right);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);
    _grid->appendRows(1);

    _gridMatrix = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _allWidgets.push_back(_gridMatrix);
    _gridMatrix->appendCollumns(3);
    _gridMatrix->appendRows(3);
    _gridMatrix->setHorizontalAlignment(iHorizontalAlignment::Left);

    _gridModifiers = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _allWidgets.push_back(_gridModifiers);
    _gridModifiers->setHorizontalAlignment(iHorizontalAlignment::Left);
    _gridModifiers->appendCollumns(3);
    _gridModifiers->appendRows(5);

    _buttonApplyTranlation = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
    _allWidgets.push_back(_buttonApplyTranlation);
    _buttonApplyTranlation->setText("Translate");
    _buttonApplyTranlation->setWidth(MV_REGULARBUTTON_SIZE);
    _buttonApplyTranlation->registerOnClickEvent(iClickDelegate(this, &UserControlTransformation::onTranslation));

    _buttonApplyScale = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
    _allWidgets.push_back(_buttonApplyScale);
    _buttonApplyScale->setText("Scale");
    _buttonApplyScale->setWidth(MV_REGULARBUTTON_SIZE);
    _buttonApplyScale->registerOnClickEvent(iClickDelegate(this, &UserControlTransformation::onScale));

    _buttonApplyID = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
    _allWidgets.push_back(_buttonApplyID);
    _buttonApplyID->setText("Identity");
    _buttonApplyID->setWidth(MV_REGULARBUTTON_SIZE);
    _buttonApplyID->registerOnClickEvent(iClickDelegate(this, &UserControlTransformation::onID));

    _buttonApplyRotateX = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
    _allWidgets.push_back(_buttonApplyRotateX);
    _buttonApplyRotateX->setText("Rotate X");
    _buttonApplyRotateX->setWidth(MV_REGULARBUTTON_SIZE);
    _buttonApplyRotateX->registerOnClickEvent(iClickDelegate(this, &UserControlTransformation::onRotationX));

    _buttonApplyRotateY = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
    _allWidgets.push_back(_buttonApplyRotateY);
    _buttonApplyRotateY->setText("Rotate Y");
    _buttonApplyRotateY->setWidth(MV_REGULARBUTTON_SIZE);
    _buttonApplyRotateY->registerOnClickEvent(iClickDelegate(this, &UserControlTransformation::onRotationY));

    _buttonApplyRotateZ = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
    _allWidgets.push_back(_buttonApplyRotateZ);
    _buttonApplyRotateZ->setText("Rotate Z");
    _buttonApplyRotateZ->setWidth(MV_REGULARBUTTON_SIZE);
    _buttonApplyRotateZ->registerOnClickEvent(iClickDelegate(this, &UserControlTransformation::onRotationZ));

    for (int i = 0; i < 3; ++i)
    {
        iWidgetTextEdit* textEdit = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget("TextEdit"));
        _allWidgets.push_back(textEdit);
        _translateText.push_back(textEdit);

        textEdit->setHorizontalTextAlignment(iHorizontalAlignment::Right);
        textEdit->setText("");
        textEdit->setWidth(MV_REGULARBUTTON_SIZE);
        textEdit->setMaxTextLength(11);
        textEdit->setHorizontalAlignment(iHorizontalAlignment::Left);
        textEdit->setVerticalAlignment(iVerticalAlignment::Top);

        _gridModifiers->addWidget(textEdit, i + 1, 1);
    }

    for (int i = 0; i < 3; ++i)
    {
        iWidgetTextEdit* textEdit = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget("TextEdit"));
        _allWidgets.push_back(textEdit);
        _scaleText.push_back(textEdit);

        textEdit->setHorizontalTextAlignment(iHorizontalAlignment::Right);
        textEdit->setText("");
        textEdit->setWidth(MV_REGULARBUTTON_SIZE);
        textEdit->setMaxTextLength(11);
        textEdit->setHorizontalAlignment(iHorizontalAlignment::Left);
        textEdit->setVerticalAlignment(iVerticalAlignment::Top);

        _gridModifiers->addWidget(textEdit, i + 1, 2);
    }

    for (int i = 0; i < 3; ++i)
    {
        iWidgetTextEdit* textEdit = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget("TextEdit"));
        _allWidgets.push_back(textEdit);
        _rotateText.push_back(textEdit);

        textEdit->setHorizontalTextAlignment(iHorizontalAlignment::Right);
        textEdit->setText("");
        textEdit->setWidth(MV_REGULARBUTTON_SIZE);
        textEdit->setMaxTextLength(11);
        textEdit->setHorizontalAlignment(iHorizontalAlignment::Left);
        textEdit->setVerticalAlignment(iVerticalAlignment::Top);

        _gridModifiers->addWidget(textEdit, 1, 3 + i);
    }

    for (int col = 0; col < 4; ++col)
    {
        for (int row = 0; row < 4; ++row)
        {
            iWidgetTextEdit* textEdit = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget("TextEdit"));
            _allWidgets.push_back(textEdit);
            _matrixText.push_back(textEdit);

            textEdit->setHorizontalTextAlignment(iHorizontalAlignment::Right);
            textEdit->setText("");
            textEdit->setWidth(MV_REGULARBUTTON_SIZE);
            textEdit->setWriteProtected(true);
            textEdit->setMaxTextLength(11);
            textEdit->setHorizontalAlignment(iHorizontalAlignment::Left);
            textEdit->setVerticalAlignment(iVerticalAlignment::Top);

            _gridMatrix->addWidget(textEdit, col, row);
        }
    }

    _grid->addWidget(_gridMatrix, 0, 0);
    _grid->addWidget(_gridModifiers, 0, 1);

    _gridModifiers->addWidget(_buttonApplyID, 0, 0);
    _gridModifiers->addWidget(_buttonApplyTranlation, 0, 1);
    _gridModifiers->addWidget(_buttonApplyScale, 0, 2);
    _gridModifiers->addWidget(_buttonApplyRotateX, 0, 3);
    _gridModifiers->addWidget(_buttonApplyRotateY, 0, 4);
    _gridModifiers->addWidget(_buttonApplyRotateZ, 0, 5);
}

void UserControlTransformation::onTranslation(iWidget* source)
{
    iNodeTransform* node = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        float32 x = iaString::atof(_translateText[0]->getText());
        float32 y = iaString::atof(_translateText[1]->getText());
        float32 z = iaString::atof(_translateText[2]->getText());

        node->translate(x, y, z);

        updateGUI();
    }
}

void UserControlTransformation::onScale(iWidget* source)
{
    iNodeTransform* node = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        float32 x = iaString::atof(_scaleText[0]->getText());
        float32 y = iaString::atof(_scaleText[1]->getText());
        float32 z = iaString::atof(_scaleText[2]->getText());

        node->scale(x, y, z);

        updateGUI();
    }
}

void UserControlTransformation::onRotationX(iWidget* source)
{
    iNodeTransform* node = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        float32 angle = iaString::atof(_rotateText[0]->getText());

        node->rotate(angle / 180.0f * M_PI, iaAxis::X);

        updateGUI();
    }
}

void UserControlTransformation::onRotationY(iWidget* source)
{
    iNodeTransform* node = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        float32 angle = iaString::atof(_rotateText[1]->getText());

        node->rotate(angle / 180.0f * M_PI, iaAxis::Y);

        updateGUI();
    }
}

void UserControlTransformation::onRotationZ(iWidget* source)
{
    iNodeTransform* node = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        float32 angle = iaString::atof(_rotateText[2]->getText());

        node->rotate(angle / 180.0f * M_PI, iaAxis::Z);

        updateGUI();
    }
}

void UserControlTransformation::onID(iWidget* source)
{
    iNodeTransform* node = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_nodeId));

    if (node != nullptr)
    {
        node->identity();
    }

    updateGUI();
}

iWidget* UserControlTransformation::getWidget()
{
    return _grid;
}