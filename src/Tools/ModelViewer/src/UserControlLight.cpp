// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlLight.h"

#include <iWidgetManager.h>
#include <iWidgetGrid.h>
#include <iWidgetTextEdit.h>
#include <iWidgetLabel.h>
#include <iWidgetButton.h>
#include <iNodeLight.h>
#include <iNodeFactory.h>
using namespace Igor;

UserControlLight::UserControlLight()
{
    initGUI();
}

UserControlLight::~UserControlLight()
{
    deinitGUI();
}

void UserControlLight::updateGUI()
{
    iNodeLight* lightNode = static_cast<iNodeLight*>(iNodeFactory::getInstance().getNode(_lightNodeId));

    if (lightNode != nullptr)
    {
        _ambient = lightNode->getAmbient();
        _diffuse = lightNode->getDiffuse();
        _specular = lightNode->getSpecular();

        _ambientColorChooser->setColor(_ambient);
        _diffuseColorChooser->setColor(_diffuse);
        _specularColorChooser->setColor(_specular);
    }
}

void UserControlLight::updateNode()
{
    iNodeLight* lightNode = static_cast<iNodeLight*>(iNodeFactory::getInstance().getNode(_lightNodeId));

    if (lightNode != nullptr)
    {
        lightNode->setAmbient(_ambient);
        lightNode->setDiffuse(_diffuse);
        lightNode->setSpecular(_specular);
    }
}

void UserControlLight::onAmbientChange(const iaColor4f& color)
{
    _ambient = color;
    updateNode();
}

void UserControlLight::onDiffuseChange(const iaColor4f& color)
{
    _diffuse = color;
    updateNode();
}

void UserControlLight::onSpecularChange(const iaColor4f& color)
{
    _specular = color;
    updateNode();
}

void UserControlLight::setNode(uint32 id)
{
    _lightNodeId = id;
    updateGUI();
}

uint32 UserControlLight::getNode()
{
    return _lightNodeId;
}

void UserControlLight::initGUI()
{
    _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    //_grid->appendCollumns(5);
    _grid->appendRows(2);
    _grid->setBorder(2);
    _grid->setHorizontalAlignment(iHorizontalAlignment::Right);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);

    _ambientColorChooser = new iUserControlColorChooser(iColorChooserMode::RGB);
    _ambientColorChooser->setText("Ambient");
    _ambientColorChooser->registerOnColorChangedEvent(iColorChangedDelegate(this, &UserControlLight::onAmbientChange));

    _diffuseColorChooser = new iUserControlColorChooser(iColorChooserMode::RGB);
    _diffuseColorChooser->setText("Diffuse");
    _diffuseColorChooser->registerOnColorChangedEvent(iColorChangedDelegate(this, &UserControlLight::onDiffuseChange));

    _specularColorChooser = new iUserControlColorChooser(iColorChooserMode::RGB);
    _specularColorChooser->setText("Specular");
    _specularColorChooser->registerOnColorChangedEvent(iColorChangedDelegate(this, &UserControlLight::onSpecularChange));

    _grid->addWidget(_ambientColorChooser->getWidget(), 0, 0);
    _grid->addWidget(_diffuseColorChooser->getWidget(), 0, 1);
    _grid->addWidget(_specularColorChooser->getWidget(), 0, 2);
}

void UserControlLight::deinitGUI()
{
    if (_grid != nullptr)
    {
        iWidgetManager::getInstance().destroyWidget(_grid);
        _grid = nullptr;
    }

    if (_ambientColorChooser != nullptr)
    {
        delete _ambientColorChooser;
        _ambientColorChooser = nullptr;
    }

    if (_diffuseColorChooser != nullptr)
    {
        delete _diffuseColorChooser;
        _diffuseColorChooser = nullptr;
    }

    if (_specularColorChooser != nullptr)
    {
        delete _specularColorChooser;
        _specularColorChooser = nullptr;
    }
}

iWidget* UserControlLight::getWidget()
{
    return _grid;
}