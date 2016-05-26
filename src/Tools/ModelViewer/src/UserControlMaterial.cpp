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
using namespace Igor;

UserControlMaterial::UserControlMaterial()
{
    initGUI();
}

UserControlMaterial::~UserControlMaterial()
{
    deinitGUI();
}

void UserControlMaterial::updateNode()
{
    iMaterial* material = iMaterialResourceFactory::getInstance().getMaterial(_materialID);

    if (material != nullptr)
    {
        material->setName(_textName->getText());
    }
}

void UserControlMaterial::updateGUI()
{
    iMaterial* material = iMaterialResourceFactory::getInstance().getMaterial(_materialID);

    if (material != nullptr)
    {
        _textName->setText(material->getName());
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

    _grid->addWidget(labelName, 0, 0);
    _grid->addWidget(_textName, 1, 0);
}

void UserControlMaterial::onTextChangedName(iWidget* source)
{
    updateNode();
    _materialNameChangedEvent();
}

void UserControlMaterial::deinitGUI()
{
    _textName->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlMaterial::onTextChangedName));

    for(auto widget : _allWidgets)
    {
        iWidgetManager::getInstance().destroyWidget(widget);
    }

    _grid = nullptr;
    _textName = nullptr;
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