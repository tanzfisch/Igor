// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlMaterial.h"

#include <iWidgetManager.h>
#include <iWidgetGrid.h>
#include <iWidgetTextEdit.h>
#include <iWidgetLabel.h>
#include <iWidgetButton.h>
#include <iWidgetSlider.h>
#include <iWidgetNumberChooser.h>
#include <iTargetMaterial.h>
#include <iMaterialResourceFactory.h>
#include <iMaterial.h>
#include <iMaterialGroup.h>
#include <iWidgetCheckBox.h>
using namespace Igor;

UserControlMaterial::UserControlMaterial()
{
    initGUI();
}

UserControlMaterial::~UserControlMaterial()
{
    deinitGUI();
}

void UserControlMaterial::updateMaterial()
{
    iMaterial* material = iMaterialResourceFactory::getInstance().getMaterial(_materialID);

    if (material != nullptr)
    {
        material->setName(_textName->getText());
        material->setOrder(static_cast<int32>(_renderingOrder->getValue()));
        material->getRenderStateSet().setRenderState(iRenderState::CullFace, _checkBoxCullFace->isChecked() ? iRenderStateValue::On : iRenderStateValue::Off);
    }
}

void UserControlMaterial::updateGUI()
{
    iMaterial* material = iMaterialResourceFactory::getInstance().getMaterial(_materialID);

    if (material != nullptr)
    {
        _textName->setText(material->getName());
        _checkBoxCullFace->setChecked(material->getRenderStateSet().getRenderStateValue(iRenderState::CullFace) == iRenderStateValue::On ? true : false);
    }
}

void UserControlMaterial::setMaterial(uint32 id)
{
    _materialID = id;
    updateGUI();
}

uint32 UserControlMaterial::getMaterial()
{
    return _materialID;
}

void UserControlMaterial::initGUI()
{
    _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(_grid);
    _grid->appendCollumns(1);
    _grid->appendRows(2);
    _grid->setBorder(2);
    _grid->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetLabel* labelName = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(labelName);
    labelName->setText("Name");
    labelName->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    _textName = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget(iWidgetType::TextEdit));
    _allWidgets.push_back(_textName);
    _textName->setMaxTextLength(100);
    _textName->setWidth(200);
    _textName->setHorrizontalAlignment(iHorrizontalAlignment::Right);
    _textName->setHorrizontalTextAlignment(iHorrizontalAlignment::Left);
    _textName->setText("...");
    _textName->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onTextChangedName));

    iWidgetLabel* labelCullFace = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(labelCullFace);
    labelCullFace->setText("Cull Face");
    labelCullFace->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    _checkBoxCullFace = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::CheckBox));
    _checkBoxCullFace->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));

    iWidgetLabel* labelRenderingOrder = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(labelRenderingOrder);
    labelRenderingOrder->setText("Rendering Order");
    labelRenderingOrder->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    _renderingOrder = static_cast<iWidgetNumberChooser*>(iWidgetManager::getInstance().createWidget(iWidgetType::NumberChooser));
    _allWidgets.push_back(_renderingOrder);
    _renderingOrder->setMinMaxNumber(iMaterial::RENDER_ORDER_MIN, iMaterial::RENDER_ORDER_MAX);
    _renderingOrder->setAfterPoint(0);
    _renderingOrder->setValue(iMaterial::RENDER_ORDER_DEFAULT);
    _renderingOrder->setWidth(80);
    _renderingOrder->setSteppingWheel(10.0f, 10.0f);
    _renderingOrder->setStepping(1.0f, 1.0f);
    _renderingOrder->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));

    _grid->addWidget(labelName, 0, 0);
    _grid->addWidget(_textName, 1, 0);
    _grid->addWidget(labelCullFace, 0, 1);
    _grid->addWidget(_checkBoxCullFace, 1, 1);
    _grid->addWidget(labelRenderingOrder, 0, 2);
    _grid->addWidget(_renderingOrder, 1, 2);
}

void UserControlMaterial::onDoUpdateMaterial(iWidget* source)
{
    updateMaterial();
}

void UserControlMaterial::onTextChangedName(iWidget* source)
{
    updateMaterial();
    _materialNameChangedEvent();
}

void UserControlMaterial::deinitGUI()
{
    _renderingOrder->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    _textName->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onTextChangedName));

    for(auto widget : _allWidgets)
    {
        iWidgetManager::getInstance().destroyWidget(widget);
    }

    _grid = nullptr;
    _textName = nullptr;
    _renderingOrder = nullptr;
}

iWidget* UserControlMaterial::getWidget()
{
    return _grid;
}

void UserControlMaterial::registerNameChangeDelegate(MaterialNameChangedDelegate nameChangedDelegate)
{
    _materialNameChangedEvent.append(nameChangedDelegate);
}

void UserControlMaterial::unregisterNameChangeDelegate(MaterialNameChangedDelegate nameChangedDelegate)
{
    _materialNameChangedEvent.remove(nameChangedDelegate);
}